/*-------------------------------------------------------------------------
 *
 * outfuncs_json.c
 *	  JSON Output functions for Postgres tree nodes.
 *
 * Portions Copyright (c) 1996-2014, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/backend/nodes/outfuncs_json.c
 *
 * NOTES
 *	  This is a copy of outfuncs.c modified to output JSON.
 *
 *    The focus of this is to make parse trees available to external tools,
 *    it is not used internally right now.
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include <ctype.h>

#include "lib/stringinfo.h"
#include "nodes/plannodes.h"
#include "nodes/relation.h"
#include "utils/datum.h"


/*
 * Macros to simplify output of different kinds of fields.	Use these
 * wherever possible to reduce the chance for silly typos.	Note that these
 * hard-wire conventions about the names of the local variables in an Out
 * routine.
 */

/* Write the label for the node type */
#define WRITE_NODE_TYPE(nodelabel) \
	appendStringInfoString(str, "\"" nodelabel "\": {")

/* Write an integer field (anything written as ":fldname %d") */
#define WRITE_INT_FIELD(fldname) \
	appendStringInfo(str, "\"" CppAsString(fldname) "\": %d, ", node->fldname)

/* Write an unsigned integer field (anything written as ":fldname %u") */
#define WRITE_UINT_FIELD(fldname) \
	appendStringInfo(str, "\"" CppAsString(fldname) "\": %u, ", node->fldname)

/* Write an OID field (don't hard-wire assumption that OID is same as uint) */
#define WRITE_OID_FIELD(fldname) \
	appendStringInfo(str, "\"" CppAsString(fldname) "\": %u, ", node->fldname)

/* Write a long-integer field */
#define WRITE_LONG_FIELD(fldname) \
	appendStringInfo(str, "\"" CppAsString(fldname) "\": %ld, ", node->fldname)

/* Write a char field (ie, one ascii character) */
#define WRITE_CHAR_FIELD(fldname) \
  if (node->fldname == 0) { appendStringInfo(str, "\"" CppAsString(fldname) "\": null, "); \
  } else { appendStringInfo(str, "\"" CppAsString(fldname) "\": \"%c\", ", node->fldname); }

/* Write an enumerated-type field as an integer code */
#define WRITE_ENUM_FIELD(fldname, enumtype) \
	appendStringInfo(str, "\"" CppAsString(fldname) "\": %d, ", \
					 (int) node->fldname)

/* Write a float field --- caller must give format to define precision */
#define WRITE_FLOAT_FIELD(fldname,format) \
	appendStringInfo(str, "\"" CppAsString(fldname) "\": " format ", ", node->fldname)

/* Write a boolean field */
#define WRITE_BOOL_FIELD(fldname) \
	appendStringInfo(str, "\"" CppAsString(fldname) "\": %s, ", \
					 booltostr(node->fldname))

/* Write a character-string (possibly NULL) field */
#define WRITE_STRING_FIELD(fldname) \
	(appendStringInfo(str, "\"" CppAsString(fldname) "\": "), \
	 _outToken(str, node->fldname), \
	 appendStringInfo(str, ", "))

/* Write a parse location field (actually same as INT case) */
#define WRITE_LOCATION_FIELD(fldname) \
	appendStringInfo(str, "\"" CppAsString(fldname) "\": %d, ", node->fldname)

/* Write a Node field */
#define WRITE_NODE_FIELD(fldname) \
	(appendStringInfo(str, "\"" CppAsString(fldname) "\": "), \
	 _outNode(str, node->fldname), \
     appendStringInfo(str, ", "))

/* Write a bitmapset field */
#define WRITE_BITMAPSET_FIELD(fldname) \
	(appendStringInfo(str, "\"" CppAsString(fldname) "\": "), \
	 _outBitmapset(str, node->fldname), \
	 appendStringInfo(str, ", "))


#define booltostr(x)  ((x) ? "true" : "false")

static void _outNode(StringInfo str, const void *obj);

static void
removeTrailingDelimiter(StringInfo str)
{
	if (str->len >= 2 && str->data[str->len - 2] == ',' && str->data[str->len - 1] == ' ') {
		str->len -= 2;
		str->data[str->len] = '\0';
	}
}

/*
 * _outToken
 *	  Convert an ordinary string (eg, an identifier) into a form that
 *	  will be decoded back to a plain token by read.c's functions.
 *
 *	  If a null or empty string is given, it is encoded as "<>".
 */
static void
_outToken(StringInfo str, const char *s)
{
	if (s == NULL || *s == '\0')
	{
		appendStringInfoString(str, "null");
		return;
	}

	appendStringInfoChar(str, '"');
	while (*s)
	{
		/* These chars must be backslashed anywhere in the string */
		if (*s == '\n')
			appendStringInfoString(str, "\\n");
		else if (*s == '\r')
			appendStringInfoString(str, "\\r");
		else if (*s == '\t')
			appendStringInfoString(str, "\\t");
		else if (*s == '\\' || *s == '"') {
			appendStringInfoChar(str, '\\');
			appendStringInfoChar(str, *s);
		} else
			appendStringInfoChar(str, *s);
		s++;
	}
	appendStringInfoChar(str, '"');
}

static void
_outList(StringInfo str, const List *node)
{
	const ListCell *lc;

	appendStringInfoChar(str, '[');

	/*if (IsA(node, IntList))
		appendStringInfoChar(str, 'i');
	else if (IsA(node, OidList))
		appendStringInfoChar(str, 'o');*/

	foreach(lc, node)
	{
		/*
		 * For the sake of backward compatibility, we emit a slightly
		 * different whitespace format for lists of nodes vs. other types of
		 * lists. XXX: is this necessary?
		 */
		if (IsA(node, List))
			_outNode(str, lfirst(lc));
		else if (IsA(node, IntList))
			appendStringInfo(str, " %d", lfirst_int(lc));
		else if (IsA(node, OidList))
			appendStringInfo(str, " %u", lfirst_oid(lc));
		else
			elog(ERROR, "unrecognized list node type: %d",
				 (int) node->type);

		if (lnext(lc))
			appendStringInfoString(str, ", ");
	}

	appendStringInfoChar(str, ']');
}

/*
 * _outBitmapset -
 *	   converts a bitmap set of integers
 *
 * Note: the output format is "(b int int ...)", similar to an integer List.
 */
static void
_outBitmapset(StringInfo str, const Bitmapset *bms)
{
	Bitmapset  *tmpset;
	int			x;

	appendStringInfoChar(str, '[');
	/*appendStringInfoChar(str, 'b');*/
	tmpset = bms_copy(bms);
	while ((x = bms_first_member(tmpset)) >= 0)
		appendStringInfo(str, "%d, ", x);
	bms_free(tmpset);
	removeTrailingDelimiter(str);
	appendStringInfoChar(str, ']');
}

/*
 * Print the value of a Datum given its type.
 */
static void
_outDatum(StringInfo str, Datum value, int typlen, bool typbyval)
{
	Size		length,
				i;
	char	   *s;

	length = datumGetSize(value, typbyval, typlen);

	if (typbyval)
	{
		s = (char *) (&value);
		/*appendStringInfo(str, "%u [ ", (unsigned int) length);*/
		appendStringInfoChar(str, '[');
		for (i = 0; i < (Size) sizeof(Datum); i++)
			appendStringInfo(str, "%d, ", (int) (s[i]));
		removeTrailingDelimiter(str);
		appendStringInfoChar(str, ']');
	}
	else
	{
		s = (char *) DatumGetPointer(value);
		if (!PointerIsValid(s))
			appendStringInfoString(str, "[]");
		else
		{
			/*appendStringInfo(str, "%u [ ", (unsigned int) length);*/
			appendStringInfoChar(str, '[');
			for (i = 0; i < length; i++)
				appendStringInfo(str, "%d, ", (int) (s[i]));
			removeTrailingDelimiter(str);
			appendStringInfoChar(str, ']');
		}
	}
}

/*
* print the basic stuff of all nodes that inherit from Path
*
* Note we do NOT print the parent, else we'd be in infinite recursion.
* We can print the parent's relids for identification purposes, though.
* We also do not print the whole of param_info, since it's printed by
* _outRelOptInfo; it's sufficient and less cluttering to print just the
* required outer relids.
*/
static void
_outPathInfo(StringInfo str, const Path *node)
{
	WRITE_ENUM_FIELD(pathtype, NodeTag);
	appendStringInfoString(str, "\"parent_relids\": ");
	_outBitmapset(str, node->parent->relids);
	appendStringInfoString(str, ", ");
	appendStringInfoString(str, "\"required_outer\": ");
	if (node->param_info)
		_outBitmapset(str, node->param_info->ppi_req_outer);
	else
		_outBitmapset(str, NULL);
	appendStringInfoString(str, ", ");
	WRITE_FLOAT_FIELD(rows, "%.0f");
	WRITE_FLOAT_FIELD(startup_cost, "%.2f");
	WRITE_FLOAT_FIELD(total_cost, "%.2f");
	WRITE_NODE_FIELD(pathkeys);
}

#include "outfuncs_shared_defs.c"

/*
 *	Stuff from plannodes.h
 */

static void
_outMergeAppend(StringInfo str, const MergeAppend *node)
{
	int			i;

	WRITE_NODE_TYPE("MERGEAPPEND");

	_outPlanInfo(str, (const Plan *) node);

	WRITE_NODE_FIELD(mergeplans);

	WRITE_INT_FIELD(numCols);

	appendStringInfoString(str, "\"sortColIdx\": [");
	for (i = 0; i < node->numCols; i++)
		appendStringInfo(str, "%d, ", node->sortColIdx[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");

	appendStringInfo(str, "\"sortOperators\": [");
	for (i = 0; i < node->numCols; i++)
		appendStringInfo(str, "%u, ", node->sortOperators[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");

	appendStringInfoString(str, "\"collations\": [");
	for (i = 0; i < node->numCols; i++)
		appendStringInfo(str, "%u, ", node->collations[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");

	appendStringInfoString(str, "\"nullsFirst\": [");
	for (i = 0; i < node->numCols; i++)
		appendStringInfo(str, "%s, ", booltostr(node->nullsFirst[i]));
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");
}

static void
_outRecursiveUnion(StringInfo str, const RecursiveUnion *node)
{
	int			i;

	WRITE_NODE_TYPE("RECURSIVEUNION");

	_outPlanInfo(str, (const Plan *) node);

	WRITE_INT_FIELD(wtParam);
	WRITE_INT_FIELD(numCols);

	appendStringInfoString(str, "\"dupColIdx\": [");
	for (i = 0; i < node->numCols; i++)
		appendStringInfo(str, "%d, ", node->dupColIdx[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");

	appendStringInfoString(str, "\"dupOperators\": [");
	for (i = 0; i < node->numCols; i++)
		appendStringInfo(str, "%u, ", node->dupOperators[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");

	WRITE_LONG_FIELD(numGroups);
}

static void
_outMergeJoin(StringInfo str, const MergeJoin *node)
{
	int			numCols;
	int			i;

	WRITE_NODE_TYPE("MERGEJOIN");

	_outJoinInfo(str, (const Join *) node);

	WRITE_NODE_FIELD(mergeclauses);

	numCols = list_length(node->mergeclauses);

	appendStringInfoString(str, "\"mergeFamilies\": [");
	for (i = 0; i < numCols; i++)
		appendStringInfo(str, "%u, ", node->mergeFamilies[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");

	appendStringInfoString(str, "\"mergeCollations\": [");
	for (i = 0; i < numCols; i++)
		appendStringInfo(str, "%u, ", node->mergeCollations[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");

	appendStringInfoString(str, "\"mergeStrategies\": [");
	for (i = 0; i < numCols; i++)
		appendStringInfo(str, "%d, ", node->mergeStrategies[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");

	appendStringInfoString(str, "\"mergeNullsFirst\": [");
	for (i = 0; i < numCols; i++)
		appendStringInfo(str, "%d, ", (int) node->mergeNullsFirst[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");
}

static void
_outAgg(StringInfo str, const Agg *node)
{
	int			i;

	WRITE_NODE_TYPE("AGG");

	_outPlanInfo(str, (const Plan *) node);

	WRITE_ENUM_FIELD(aggstrategy, AggStrategy);
	WRITE_INT_FIELD(numCols);

	appendStringInfoString(str, "\"grpColIdx\": [");
	for (i = 0; i < node->numCols; i++)
		appendStringInfo(str, "%d, ", node->grpColIdx[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");

	appendStringInfoString(str, "\"grpOperators\": [");
	for (i = 0; i < node->numCols; i++)
		appendStringInfo(str, "%u, ", node->grpOperators[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");

	WRITE_LONG_FIELD(numGroups);
}

static void
_outWindowAgg(StringInfo str, const WindowAgg *node)
{
	int			i;

	WRITE_NODE_TYPE("WINDOWAGG");

	_outPlanInfo(str, (const Plan *) node);

	WRITE_UINT_FIELD(winref);
	WRITE_INT_FIELD(partNumCols);

	appendStringInfoString(str, "\"partColIdx\": [");
	for (i = 0; i < node->partNumCols; i++)
		appendStringInfo(str, "%d, ", node->partColIdx[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");

	appendStringInfoString(str, "\"partOperations\": [");
	for (i = 0; i < node->partNumCols; i++)
		appendStringInfo(str, "%u, ", node->partOperators[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");

	WRITE_INT_FIELD(ordNumCols);

	appendStringInfoString(str, "\"ordColIdx\": [");
	for (i = 0; i < node->ordNumCols; i++)
		appendStringInfo(str, "%d, ", node->ordColIdx[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");

	appendStringInfoString(str, "\"ordOperations\": [");
	for (i = 0; i < node->ordNumCols; i++)
		appendStringInfo(str, "%u, ", node->ordOperators[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");

	WRITE_INT_FIELD(frameOptions);
	WRITE_NODE_FIELD(startOffset);
	WRITE_NODE_FIELD(endOffset);
}

static void
_outGroup(StringInfo str, const Group *node)
{
	int			i;

	WRITE_NODE_TYPE("GROUP");

	_outPlanInfo(str, (const Plan *) node);

	WRITE_INT_FIELD(numCols);

	appendStringInfoString(str, "\"grpColIdx\": [");
	for (i = 0; i < node->numCols; i++)
		appendStringInfo(str, "%d, ", node->grpColIdx[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");

	appendStringInfoString(str, "\"grpOperators\": [");
	for (i = 0; i < node->numCols; i++)
		appendStringInfo(str, "%u, ", node->grpOperators[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");
}

static void
_outSort(StringInfo str, const Sort *node)
{
	int			i;

	WRITE_NODE_TYPE("SORT");

	_outPlanInfo(str, (const Plan *) node);

	WRITE_INT_FIELD(numCols);

	appendStringInfoString(str, "\"sortColIdx\": [");
	for (i = 0; i < node->numCols; i++)
		appendStringInfo(str, "%d, ", node->sortColIdx[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");

	appendStringInfoString(str, "\"sortOperators\": [");
	for (i = 0; i < node->numCols; i++)
		appendStringInfo(str, "%u, ", node->sortOperators[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");

	appendStringInfoString(str, "\"collations\": [");
	for (i = 0; i < node->numCols; i++)
		appendStringInfo(str, "%u, ", node->collations[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");

	appendStringInfoString(str, "\"nullsFirst\": [");
	for (i = 0; i < node->numCols; i++)
		appendStringInfo(str, "%s, ", booltostr(node->nullsFirst[i]));
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");
}

static void
_outUnique(StringInfo str, const Unique *node)
{
	int			i;

	WRITE_NODE_TYPE("UNIQUE");

	_outPlanInfo(str, (const Plan *) node);

	WRITE_INT_FIELD(numCols);

	appendStringInfoString(str, "\"uniqColIdx\": [");
	for (i = 0; i < node->numCols; i++)
		appendStringInfo(str, "%d, ", node->uniqColIdx[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");

	appendStringInfoString(str, "\"uniqOperators\": [");
	for (i = 0; i < node->numCols; i++)
		appendStringInfo(str, "%u, ", node->uniqOperators[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");
}

static void
_outSetOp(StringInfo str, const SetOp *node)
{
	int			i;

	WRITE_NODE_TYPE("SETOP");

	_outPlanInfo(str, (const Plan *) node);

	WRITE_ENUM_FIELD(cmd, SetOpCmd);
	WRITE_ENUM_FIELD(strategy, SetOpStrategy);
	WRITE_INT_FIELD(numCols);

	appendStringInfoString(str, "\"dupColIdx\": [");
	for (i = 0; i < node->numCols; i++)
		appendStringInfo(str, "%d, ", node->dupColIdx[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");

	appendStringInfoString(str, "\"dupOperators\": [");
	for (i = 0; i < node->numCols; i++)
		appendStringInfo(str, "%u, ", node->dupOperators[i]);
	removeTrailingDelimiter(str);
	appendStringInfoString(str, "], ");

	WRITE_INT_FIELD(flagColIdx);
	WRITE_INT_FIELD(firstFlag);
	WRITE_LONG_FIELD(numGroups);
}

/*****************************************************************************
 *
 *	Stuff from primnodes.h.
 *
 *****************************************************************************/

static void
_outConst(StringInfo str, const Const *node)
{
	WRITE_NODE_TYPE("CONST");

	WRITE_OID_FIELD(consttype);
	WRITE_INT_FIELD(consttypmod);
	WRITE_OID_FIELD(constcollid);
	WRITE_INT_FIELD(constlen);
	WRITE_BOOL_FIELD(constbyval);
	WRITE_BOOL_FIELD(constisnull);
	WRITE_LOCATION_FIELD(location);

	appendStringInfoString(str, "\"constvalue\": ");
	if (node->constisnull)
		appendStringInfoString(str, "null");
	else
		_outDatum(str, node->constvalue, node->constlen, node->constbyval);
	appendStringInfoString(str, ", ");
}

static void
_outBoolExpr(StringInfo str, const BoolExpr *node)
{
	char	   *opstr = NULL;

	WRITE_NODE_TYPE("BOOLEXPR");

	/* do-it-yourself enum representation */
	switch (node->boolop)
	{
		case AND_EXPR:
			opstr = "and";
			break;
		case OR_EXPR:
			opstr = "or";
			break;
		case NOT_EXPR:
			opstr = "not";
			break;
	}
	appendStringInfoString(str, "\"boolop\": ");
	_outToken(str, opstr);
	appendStringInfoString(str, ", ");

	WRITE_NODE_FIELD(args);
	WRITE_LOCATION_FIELD(location);
}

/*****************************************************************************
 *
 *	Stuff from relation.h.
 *
 *****************************************************************************/

static void
_outPath(StringInfo str, const Path *node)
{
	WRITE_NODE_TYPE("PATH");

	_outPathInfo(str, (const Path *) node);
}

static void
_outEquivalenceClass(StringInfo str, const EquivalenceClass *node)
{
	/*
	 * To simplify reading, we just chase up to the topmost merged EC and
	 * print that, without bothering to show the merge-ees separately.
	 */
	while (node->ec_merged)
		node = node->ec_merged;

	WRITE_NODE_TYPE("EQUIVALENCECLASS");

	WRITE_NODE_FIELD(ec_opfamilies);
	WRITE_OID_FIELD(ec_collation);
	WRITE_NODE_FIELD(ec_members);
	WRITE_NODE_FIELD(ec_sources);
	WRITE_NODE_FIELD(ec_derives);
	WRITE_BITMAPSET_FIELD(ec_relids);
	WRITE_BOOL_FIELD(ec_has_const);
	WRITE_BOOL_FIELD(ec_has_volatile);
	WRITE_BOOL_FIELD(ec_below_outer_join);
	WRITE_BOOL_FIELD(ec_broken);
	WRITE_UINT_FIELD(ec_sortref);
}

/*****************************************************************************
 *
 *	Stuff from parsenodes.h.
 *
 *****************************************************************************/

static void
_outRangeTblEntry(StringInfo str, const RangeTblEntry *node)
{
	WRITE_NODE_TYPE("RTE");

	/* put alias + eref first to make dump more legible */
	WRITE_NODE_FIELD(alias);
	WRITE_NODE_FIELD(eref);
	WRITE_ENUM_FIELD(rtekind, RTEKind);

	switch (node->rtekind)
	{
		case RTE_RELATION:
			WRITE_OID_FIELD(relid);
			WRITE_CHAR_FIELD(relkind);
			break;
		case RTE_SUBQUERY:
			WRITE_NODE_FIELD(subquery);
			WRITE_BOOL_FIELD(security_barrier);
			break;
		case RTE_JOIN:
			WRITE_ENUM_FIELD(jointype, JoinType);
			WRITE_NODE_FIELD(joinaliasvars);
			break;
		case RTE_FUNCTION:
			WRITE_NODE_FIELD(functions);
			WRITE_BOOL_FIELD(funcordinality);
			break;
		case RTE_VALUES:
			WRITE_NODE_FIELD(values_lists);
			WRITE_NODE_FIELD(values_collations);
			break;
		case RTE_CTE:
			WRITE_STRING_FIELD(ctename);
			WRITE_UINT_FIELD(ctelevelsup);
			WRITE_BOOL_FIELD(self_reference);
			WRITE_NODE_FIELD(ctecoltypes);
			WRITE_NODE_FIELD(ctecoltypmods);
			WRITE_NODE_FIELD(ctecolcollations);
			break;
		default:
			elog(ERROR, "unrecognized RTE kind: %d", (int) node->rtekind);
			break;
	}

	WRITE_BOOL_FIELD(lateral);
	WRITE_BOOL_FIELD(inh);
	WRITE_BOOL_FIELD(inFromCl);
	WRITE_UINT_FIELD(requiredPerms);
	WRITE_OID_FIELD(checkAsUser);
	WRITE_BITMAPSET_FIELD(selectedCols);
	WRITE_BITMAPSET_FIELD(modifiedCols);
	WRITE_NODE_FIELD(securityQuals);
}

static void
_outAExpr(StringInfo str, const A_Expr *node)
{
	switch (node->kind)
	{
		case AEXPR_OP:
			WRITE_NODE_TYPE("AEXPR");
			WRITE_NODE_FIELD(name);
			break;
		case AEXPR_AND:
			WRITE_NODE_TYPE("AEXPR AND");
			break;
		case AEXPR_OR:
			WRITE_NODE_TYPE("AEXPR OR");
			break;
		case AEXPR_NOT:
			WRITE_NODE_TYPE("AEXPR NOT");
			break;
		case AEXPR_OP_ANY:
			WRITE_NODE_TYPE("AEXPR ANY");
			WRITE_NODE_FIELD(name);
			break;
		case AEXPR_OP_ALL:
			WRITE_NODE_TYPE("AEXPR ALL");
			WRITE_NODE_FIELD(name);
			break;
		case AEXPR_DISTINCT:
			WRITE_NODE_TYPE("AEXPR DISTINCT");
			WRITE_NODE_FIELD(name);
			break;
		case AEXPR_NULLIF:
			WRITE_NODE_TYPE("AEXPR NULLIF");
			WRITE_NODE_FIELD(name);
			break;
		case AEXPR_OF:
			WRITE_NODE_TYPE("AEXPR OF");
			WRITE_NODE_FIELD(name);
			break;
		case AEXPR_IN:
			WRITE_NODE_TYPE("AEXPR IN");
			WRITE_NODE_FIELD(name);
			break;
		default:
			WRITE_NODE_TYPE("AEXPR ??");
			break;
	}

	WRITE_NODE_FIELD(lexpr);
	WRITE_NODE_FIELD(rexpr);
	WRITE_LOCATION_FIELD(location);
}

static void
_outValue(StringInfo str, const Value *value)
{
	switch (value->type)
	{
		case T_Integer:
			appendStringInfo(str, "%ld", value->val.ival);
			break;
		case T_Float:

			/*
			 * We assume the value is a valid numeric literal and so does not
			 * need quoting.
			 */
			if (strlen(value->val.str) > 0 && value->val.str[0] == '.')
				appendStringInfoChar(str, '0');
			appendStringInfoString(str, value->val.str);
			if (strlen(value->val.str) > 0 && value->val.str[strlen(value->val.str) - 1] == '.')
				appendStringInfoChar(str, '0');
			break;
		case T_String:
			_outToken(str, value->val.str);
			break;
		case T_BitString:
			/* internal representation already has leading 'b' */
			appendStringInfoString(str, value->val.str);
			break;
		case T_Null:
			/* this is seen only within A_Const, not in transformed trees */
			appendStringInfoString(str, "null");
			break;
		default:
			elog(ERROR, "unrecognized node type: %d", (int) value->type);
			break;
	}
}

static void
_outAConst(StringInfo str, const A_Const *node)
{
	WRITE_NODE_TYPE("A_CONST");

	appendStringInfoString(str, "\"val\": ");
	_outValue(str, &(node->val));
	appendStringInfoString(str, ", ");
	WRITE_LOCATION_FIELD(location);
}

static void
_outConstraint(StringInfo str, const Constraint *node)
{
	WRITE_NODE_TYPE("CONSTRAINT");

	WRITE_STRING_FIELD(conname);
	WRITE_BOOL_FIELD(deferrable);
	WRITE_BOOL_FIELD(initdeferred);
	WRITE_LOCATION_FIELD(location);

	switch (node->contype)
	{
		case CONSTR_NULL:
			appendStringInfoString(str, "\"contype\": \"NULL\", ");
			break;

		case CONSTR_NOTNULL:
			appendStringInfoString(str, "\"contype\": \"NOT_NULL\", ");
			break;

		case CONSTR_DEFAULT:
			appendStringInfoString(str, "\"contype\": \"DEFAULT\", ");
			WRITE_NODE_FIELD(raw_expr);
			WRITE_STRING_FIELD(cooked_expr);
			break;

		case CONSTR_CHECK:
			appendStringInfoString(str, "\"contype\": \"CHECK\", ");
			WRITE_BOOL_FIELD(is_no_inherit);
			WRITE_NODE_FIELD(raw_expr);
			WRITE_STRING_FIELD(cooked_expr);
			break;

		case CONSTR_PRIMARY:
			appendStringInfoString(str, "\"contype\": \"PRIMARY_KEY\", ");
			WRITE_NODE_FIELD(keys);
			WRITE_NODE_FIELD(options);
			WRITE_STRING_FIELD(indexname);
			WRITE_STRING_FIELD(indexspace);
			/* access_method and where_clause not currently used */
			break;

		case CONSTR_UNIQUE:
			appendStringInfoString(str, "\"contype\": \"UNIQUE\", ");
			WRITE_NODE_FIELD(keys);
			WRITE_NODE_FIELD(options);
			WRITE_STRING_FIELD(indexname);
			WRITE_STRING_FIELD(indexspace);
			/* access_method and where_clause not currently used */
			break;

		case CONSTR_EXCLUSION:
			appendStringInfoString(str, "\"contype\": \"EXCLUSION\", ");
			WRITE_NODE_FIELD(exclusions);
			WRITE_NODE_FIELD(options);
			WRITE_STRING_FIELD(indexname);
			WRITE_STRING_FIELD(indexspace);
			WRITE_STRING_FIELD(access_method);
			WRITE_NODE_FIELD(where_clause);
			break;

		case CONSTR_FOREIGN:
			appendStringInfoString(str, "\"contype\": \"FOREIGN_KEY\", ");
			WRITE_NODE_FIELD(pktable);
			WRITE_NODE_FIELD(fk_attrs);
			WRITE_NODE_FIELD(pk_attrs);
			WRITE_CHAR_FIELD(fk_matchtype);
			WRITE_CHAR_FIELD(fk_upd_action);
			WRITE_CHAR_FIELD(fk_del_action);
			WRITE_NODE_FIELD(old_conpfeqop);
			WRITE_OID_FIELD(old_pktable_oid);
			WRITE_BOOL_FIELD(skip_validation);
			WRITE_BOOL_FIELD(initially_valid);
			break;

		case CONSTR_ATTR_DEFERRABLE:
			appendStringInfoString(str, "\"contype\": \"ATTR_DEFERRABLE\", ");
			break;

		case CONSTR_ATTR_NOT_DEFERRABLE:
			appendStringInfoString(str, "\"contype\": \"ATTR_NOT_DEFERRABLE\", ");
			break;

		case CONSTR_ATTR_DEFERRED:
			appendStringInfoString(str, "\"contype\": \"ATTR_DEFERRED\", ");
			break;

		case CONSTR_ATTR_IMMEDIATE:
			appendStringInfoString(str, "\"contype\": \"ATTR_IMMEDIATE\", ");
			break;

		default:
			/*appendStringInfo(str, "<unrecognized_constraint %d>",
							 (int) node->contype);*/
			appendStringInfoString(str, "\"contype\": null, ");
			break;
	}
}

/*
 * _outNode -
 *	  converts a Node into ascii string and append it to 'str'
 */
static void
_outNode(StringInfo str, const void *obj)
{
	if (obj == NULL)
		appendStringInfoString(str, "null");
	else if (IsA(obj, List) ||IsA(obj, IntList) || IsA(obj, OidList))
		_outList(str, obj);
	else if (IsA(obj, Integer) ||
			 IsA(obj, Float) ||
			 IsA(obj, String) ||
			 IsA(obj, BitString))
	{
		/* nodeRead does not want to see { } around these! */
		_outValue(str, obj);
	}
	else
	{
		appendStringInfoChar(str, '{');
		switch (nodeTag(obj))
		{
			case T_MergeAppend:
				_outMergeAppend(str, obj);
				break;
			case T_RecursiveUnion:
				_outRecursiveUnion(str, obj);
				break;
			case T_MergeJoin:
				_outMergeJoin(str, obj);
				break;
			case T_Agg:
				_outAgg(str, obj);
				break;
			case T_WindowAgg:
				_outWindowAgg(str, obj);
				break;
			case T_Group:
				_outGroup(str, obj);
				break;
			case T_Sort:
				_outSort(str, obj);
				break;
			case T_Unique:
				_outUnique(str, obj);
				break;
			case T_SetOp:
				_outSetOp(str, obj);
				break;
			case T_Const:
				_outConst(str, obj);
				break;
			case T_BoolExpr:
				_outBoolExpr(str, obj);
				break;
			case T_Path:
				_outPath(str, obj);
				break;
			case T_EquivalenceClass:
				_outEquivalenceClass(str, obj);
				break;
			case T_RangeTblEntry:
				_outRangeTblEntry(str, obj);
				break;
			case T_A_Expr:
				_outAExpr(str, obj);
				break;
			case T_A_Const:
				_outAConst(str, obj);
				break;
			case T_Constraint:
				_outConstraint(str, obj);
				break;

			#include "outfuncs_shared_conds.c"

			default:

				/*
				 * This should be an ERROR, but it's too useful to be able to
				 * dump structures that _outNode only understands part of.
				 */
				elog(WARNING, "could not dump unrecognized node type: %d",
					 (int) nodeTag(obj));

				appendStringInfo(str, "}");
				return;
		}
		removeTrailingDelimiter(str);
		appendStringInfo(str, "}}");
	}
}

/*
 * nodeToJSONString -
 *	   returns the JSON representation of the Node as a palloc'd string
 */
char *
nodeToJSONString(const void *obj)
{
	StringInfoData str;

	/* see stringinfo.h for an explanation of this maneuver */
	initStringInfo(&str);

	if (obj == NULL) /* Make sure we generate valid JSON for empty queries */
		appendStringInfoString(&str, "[]");
	else
		_outNode(&str, obj);

	return str.data;
}
