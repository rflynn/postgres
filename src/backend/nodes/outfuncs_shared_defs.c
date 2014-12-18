static void
_outPlanInfo(StringInfo str, const Plan *node)
{
  WRITE_FLOAT_FIELD(startup_cost, "%.2f");
  WRITE_FLOAT_FIELD(total_cost, "%.2f");
  WRITE_FLOAT_FIELD(plan_rows, "%.0f");
  WRITE_INT_FIELD(plan_width);
  WRITE_NODE_FIELD(targetlist);
  WRITE_NODE_FIELD(qual);
  WRITE_NODE_FIELD(lefttree);
  WRITE_NODE_FIELD(righttree);
  WRITE_NODE_FIELD(initPlan);
  WRITE_BITMAPSET_FIELD(extParam);
  WRITE_BITMAPSET_FIELD(allParam);
}

static void
_outScanInfo(StringInfo str, const Scan *node)
{
  _outPlanInfo(str, (const Plan *) node);

  WRITE_UINT_FIELD(scanrelid);
}

static void
_outJoinInfo(StringInfo str, const Join *node)
{
  _outPlanInfo(str, (const Plan *) node);

  WRITE_ENUM_FIELD(jointype, JoinType);
  WRITE_NODE_FIELD(joinqual);
}

static void
_outJoinPathInfo(StringInfo str, const JoinPath *node)
{
  _outPathInfo(str, (const Path *) node);

  WRITE_ENUM_FIELD(jointype, JoinType);
  WRITE_NODE_FIELD(outerjoinpath);
  WRITE_NODE_FIELD(innerjoinpath);
  WRITE_NODE_FIELD(joinrestrictinfo);
}

static void
_outCreateStmtInfo(StringInfo str, const CreateStmt *node)
{
  WRITE_NODE_FIELD(relation);
  WRITE_NODE_FIELD(tableElts);
  WRITE_NODE_FIELD(inhRelations);
  WRITE_NODE_FIELD(ofTypename);
  WRITE_NODE_FIELD(constraints);
  WRITE_NODE_FIELD(options);
  WRITE_ENUM_FIELD(oncommit, OnCommitAction);
  WRITE_STRING_FIELD(tablespacename);
  WRITE_BOOL_FIELD(if_not_exists);
}

static void
_outPlan(StringInfo str, const Plan *node)
{
  WRITE_NODE_TYPE("PLAN");

  _outPlanInfo(str, (const Plan *) node);

}

static void
_outResult(StringInfo str, const Result *node)
{
  WRITE_NODE_TYPE("RESULT");

  _outPlanInfo(str, (const Plan *) node);

  WRITE_NODE_FIELD(resconstantqual);
}

static void
_outModifyTable(StringInfo str, const ModifyTable *node)
{
  WRITE_NODE_TYPE("MODIFYTABLE");

  _outPlanInfo(str, (const Plan *) node);

  WRITE_ENUM_FIELD(operation, CmdType);
  WRITE_BOOL_FIELD(canSetTag);
  WRITE_NODE_FIELD(resultRelations);
  WRITE_INT_FIELD(resultRelIndex);
  WRITE_NODE_FIELD(plans);
  WRITE_NODE_FIELD(withCheckOptionLists);
  WRITE_NODE_FIELD(returningLists);
  WRITE_NODE_FIELD(fdwPrivLists);
  WRITE_NODE_FIELD(rowMarks);
  WRITE_INT_FIELD(epqParam);
}

static void
_outAppend(StringInfo str, const Append *node)
{
  WRITE_NODE_TYPE("APPEND");

  _outPlanInfo(str, (const Plan *) node);

  WRITE_NODE_FIELD(appendplans);
}

static void
_outBitmapAnd(StringInfo str, const BitmapAnd *node)
{
  WRITE_NODE_TYPE("BITMAPAND");

  _outPlanInfo(str, (const Plan *) node);

  WRITE_NODE_FIELD(bitmapplans);
}

static void
_outBitmapOr(StringInfo str, const BitmapOr *node)
{
  WRITE_NODE_TYPE("BITMAPOR");

  _outPlanInfo(str, (const Plan *) node);

  WRITE_NODE_FIELD(bitmapplans);
}

static void
_outScan(StringInfo str, const Scan *node)
{
  WRITE_NODE_TYPE("SCAN");

  _outScanInfo(str, (const Scan *) node);

}

static void
_outSeqScan(StringInfo str, const SeqScan *node)
{
  WRITE_NODE_TYPE("SEQSCAN");

  _outScanInfo(str, (const Scan *) node);

}

static void
_outIndexScan(StringInfo str, const IndexScan *node)
{
  WRITE_NODE_TYPE("INDEXSCAN");

  _outScanInfo(str, (const Scan *) node);

  WRITE_OID_FIELD(indexid);
  WRITE_NODE_FIELD(indexqual);
  WRITE_NODE_FIELD(indexqualorig);
  WRITE_NODE_FIELD(indexorderby);
  WRITE_NODE_FIELD(indexorderbyorig);
  WRITE_ENUM_FIELD(indexorderdir, ScanDirection);
}

static void
_outIndexOnlyScan(StringInfo str, const IndexOnlyScan *node)
{
  WRITE_NODE_TYPE("INDEXONLYSCAN");

  _outScanInfo(str, (const Scan *) node);

  WRITE_OID_FIELD(indexid);
  WRITE_NODE_FIELD(indexqual);
  WRITE_NODE_FIELD(indexorderby);
  WRITE_NODE_FIELD(indextlist);
  WRITE_ENUM_FIELD(indexorderdir, ScanDirection);
}

static void
_outBitmapIndexScan(StringInfo str, const BitmapIndexScan *node)
{
  WRITE_NODE_TYPE("BITMAPINDEXSCAN");

  _outScanInfo(str, (const Scan *) node);

  WRITE_OID_FIELD(indexid);
  WRITE_NODE_FIELD(indexqual);
  WRITE_NODE_FIELD(indexqualorig);
}

static void
_outBitmapHeapScan(StringInfo str, const BitmapHeapScan *node)
{
  WRITE_NODE_TYPE("BITMAPHEAPSCAN");

  _outScanInfo(str, (const Scan *) node);

  WRITE_NODE_FIELD(bitmapqualorig);
}

static void
_outTidScan(StringInfo str, const TidScan *node)
{
  WRITE_NODE_TYPE("TIDSCAN");

  _outScanInfo(str, (const Scan *) node);

  WRITE_NODE_FIELD(tidquals);
}

static void
_outSubqueryScan(StringInfo str, const SubqueryScan *node)
{
  WRITE_NODE_TYPE("SUBQUERYSCAN");

  _outScanInfo(str, (const Scan *) node);

  WRITE_NODE_FIELD(subplan);
}

static void
_outFunctionScan(StringInfo str, const FunctionScan *node)
{
  WRITE_NODE_TYPE("FUNCTIONSCAN");

  _outScanInfo(str, (const Scan *) node);

  WRITE_NODE_FIELD(functions);
  WRITE_BOOL_FIELD(funcordinality);
}

static void
_outValuesScan(StringInfo str, const ValuesScan *node)
{
  WRITE_NODE_TYPE("VALUESSCAN");

  _outScanInfo(str, (const Scan *) node);

  WRITE_NODE_FIELD(values_lists);
}

static void
_outCteScan(StringInfo str, const CteScan *node)
{
  WRITE_NODE_TYPE("CTESCAN");

  _outScanInfo(str, (const Scan *) node);

  WRITE_INT_FIELD(ctePlanId);
  WRITE_INT_FIELD(cteParam);
}

static void
_outWorkTableScan(StringInfo str, const WorkTableScan *node)
{
  WRITE_NODE_TYPE("WORKTABLESCAN");

  _outScanInfo(str, (const Scan *) node);

  WRITE_INT_FIELD(wtParam);
}

static void
_outForeignScan(StringInfo str, const ForeignScan *node)
{
  WRITE_NODE_TYPE("FOREIGNSCAN");

  _outScanInfo(str, (const Scan *) node);

  WRITE_NODE_FIELD(fdw_exprs);
  WRITE_NODE_FIELD(fdw_private);
  WRITE_BOOL_FIELD(fsSystemCol);
}

static void
_outJoin(StringInfo str, const Join *node)
{
  WRITE_NODE_TYPE("JOIN");

  _outJoinInfo(str, (const Join *) node);

}

static void
_outNestLoop(StringInfo str, const NestLoop *node)
{
  WRITE_NODE_TYPE("NESTLOOP");

  _outJoinInfo(str, (const Join *) node);

  WRITE_NODE_FIELD(nestParams);
}

static void
_outHashJoin(StringInfo str, const HashJoin *node)
{
  WRITE_NODE_TYPE("HASHJOIN");

  _outJoinInfo(str, (const Join *) node);

  WRITE_NODE_FIELD(hashclauses);
}

static void
_outMaterial(StringInfo str, const Material *node)
{
  WRITE_NODE_TYPE("MATERIAL");

  _outPlanInfo(str, (const Plan *) node);

}

static void
_outHash(StringInfo str, const Hash *node)
{
  WRITE_NODE_TYPE("HASH");

  _outPlanInfo(str, (const Plan *) node);

  WRITE_OID_FIELD(skewTable);
  WRITE_INT_FIELD(skewColumn);
  WRITE_BOOL_FIELD(skewInherit);
  WRITE_OID_FIELD(skewColType);
  WRITE_INT_FIELD(skewColTypmod);
}

static void
_outLockRows(StringInfo str, const LockRows *node)
{
  WRITE_NODE_TYPE("LOCKROWS");

  _outPlanInfo(str, (const Plan *) node);

  WRITE_NODE_FIELD(rowMarks);
  WRITE_INT_FIELD(epqParam);
}

static void
_outLimit(StringInfo str, const Limit *node)
{
  WRITE_NODE_TYPE("LIMIT");

  _outPlanInfo(str, (const Plan *) node);

  WRITE_NODE_FIELD(limitOffset);
  WRITE_NODE_FIELD(limitCount);
}

static void
_outNestLoopParam(StringInfo str, const NestLoopParam *node)
{
  WRITE_NODE_TYPE("NESTLOOPPARAM");

  WRITE_INT_FIELD(paramno);
  WRITE_NODE_FIELD(paramval);
}

static void
_outPlanRowMark(StringInfo str, const PlanRowMark *node)
{
  WRITE_NODE_TYPE("PLANROWMARK");

  WRITE_UINT_FIELD(rti);
  WRITE_UINT_FIELD(prti);
  WRITE_UINT_FIELD(rowmarkId);
  WRITE_ENUM_FIELD(markType, RowMarkType);
  WRITE_BOOL_FIELD(noWait);
  WRITE_BOOL_FIELD(isParent);
}

static void
_outPlanInvalItem(StringInfo str, const PlanInvalItem *node)
{
  WRITE_NODE_TYPE("PLANINVALITEM");

  WRITE_INT_FIELD(cacheId);
  WRITE_UINT_FIELD(hashValue);
}

static void
_outAlias(StringInfo str, const Alias *node)
{
  WRITE_NODE_TYPE("ALIAS");

  WRITE_STRING_FIELD(aliasname);
  WRITE_NODE_FIELD(colnames);
}

static void
_outRangeVar(StringInfo str, const RangeVar *node)
{
  WRITE_NODE_TYPE("RANGEVAR");

  WRITE_STRING_FIELD(schemaname);
  WRITE_STRING_FIELD(relname);
  WRITE_ENUM_FIELD(inhOpt, InhOption);
  WRITE_CHAR_FIELD(relpersistence);
  WRITE_NODE_FIELD(alias);
  WRITE_LOCATION_FIELD(location);
}

static void
_outVar(StringInfo str, const Var *node)
{
  WRITE_NODE_TYPE("VAR");

  WRITE_UINT_FIELD(varno);
  WRITE_INT_FIELD(varattno);
  WRITE_OID_FIELD(vartype);
  WRITE_INT_FIELD(vartypmod);
  WRITE_OID_FIELD(varcollid);
  WRITE_UINT_FIELD(varlevelsup);
  WRITE_UINT_FIELD(varnoold);
  WRITE_INT_FIELD(varoattno);
  WRITE_LOCATION_FIELD(location);
}

static void
_outParam(StringInfo str, const Param *node)
{
  WRITE_NODE_TYPE("PARAM");

  WRITE_ENUM_FIELD(paramkind, ParamKind);
  WRITE_INT_FIELD(paramid);
  WRITE_OID_FIELD(paramtype);
  WRITE_INT_FIELD(paramtypmod);
  WRITE_OID_FIELD(paramcollid);
  WRITE_LOCATION_FIELD(location);
}

static void
_outAggref(StringInfo str, const Aggref *node)
{
  WRITE_NODE_TYPE("AGGREF");

  WRITE_OID_FIELD(aggfnoid);
  WRITE_OID_FIELD(aggtype);
  WRITE_OID_FIELD(aggcollid);
  WRITE_OID_FIELD(inputcollid);
  WRITE_NODE_FIELD(aggdirectargs);
  WRITE_NODE_FIELD(args);
  WRITE_NODE_FIELD(aggorder);
  WRITE_NODE_FIELD(aggdistinct);
  WRITE_NODE_FIELD(aggfilter);
  WRITE_BOOL_FIELD(aggstar);
  WRITE_BOOL_FIELD(aggvariadic);
  WRITE_CHAR_FIELD(aggkind);
  WRITE_UINT_FIELD(agglevelsup);
  WRITE_LOCATION_FIELD(location);
}

static void
_outWindowFunc(StringInfo str, const WindowFunc *node)
{
  WRITE_NODE_TYPE("WINDOWFUNC");

  WRITE_OID_FIELD(winfnoid);
  WRITE_OID_FIELD(wintype);
  WRITE_OID_FIELD(wincollid);
  WRITE_OID_FIELD(inputcollid);
  WRITE_NODE_FIELD(args);
  WRITE_NODE_FIELD(aggfilter);
  WRITE_UINT_FIELD(winref);
  WRITE_BOOL_FIELD(winstar);
  WRITE_BOOL_FIELD(winagg);
  WRITE_LOCATION_FIELD(location);
}

static void
_outArrayRef(StringInfo str, const ArrayRef *node)
{
  WRITE_NODE_TYPE("ARRAYREF");

  WRITE_OID_FIELD(refarraytype);
  WRITE_OID_FIELD(refelemtype);
  WRITE_INT_FIELD(reftypmod);
  WRITE_OID_FIELD(refcollid);
  WRITE_NODE_FIELD(refupperindexpr);
  WRITE_NODE_FIELD(reflowerindexpr);
  WRITE_NODE_FIELD(refexpr);
  WRITE_NODE_FIELD(refassgnexpr);
}

static void
_outFuncExpr(StringInfo str, const FuncExpr *node)
{
  WRITE_NODE_TYPE("FUNCEXPR");

  WRITE_OID_FIELD(funcid);
  WRITE_OID_FIELD(funcresulttype);
  WRITE_BOOL_FIELD(funcretset);
  WRITE_BOOL_FIELD(funcvariadic);
  WRITE_ENUM_FIELD(funcformat, CoercionForm);
  WRITE_OID_FIELD(funccollid);
  WRITE_OID_FIELD(inputcollid);
  WRITE_NODE_FIELD(args);
  WRITE_LOCATION_FIELD(location);
}

static void
_outNamedArgExpr(StringInfo str, const NamedArgExpr *node)
{
  WRITE_NODE_TYPE("NAMEDARGEXPR");

  WRITE_NODE_FIELD(arg);
  WRITE_STRING_FIELD(name);
  WRITE_INT_FIELD(argnumber);
  WRITE_LOCATION_FIELD(location);
}

static void
_outOpExpr(StringInfo str, const OpExpr *node)
{
  WRITE_NODE_TYPE("OPEXPR");

  WRITE_OID_FIELD(opno);
  WRITE_OID_FIELD(opfuncid);
  WRITE_OID_FIELD(opresulttype);
  WRITE_BOOL_FIELD(opretset);
  WRITE_OID_FIELD(opcollid);
  WRITE_OID_FIELD(inputcollid);
  WRITE_NODE_FIELD(args);
  WRITE_LOCATION_FIELD(location);
}

static void
_outDistinctExpr(StringInfo str, const DistinctExpr *node)
{
  WRITE_NODE_TYPE("DISTINCTEXPR");

  WRITE_OID_FIELD(opno);
  WRITE_OID_FIELD(opfuncid);
  WRITE_OID_FIELD(opresulttype);
  WRITE_BOOL_FIELD(opretset);
  WRITE_OID_FIELD(opcollid);
  WRITE_OID_FIELD(inputcollid);
  WRITE_NODE_FIELD(args);
  WRITE_LOCATION_FIELD(location);
}

static void
_outNullIfExpr(StringInfo str, const NullIfExpr *node)
{
  WRITE_NODE_TYPE("NULLIFEXPR");

  WRITE_OID_FIELD(opno);
  WRITE_OID_FIELD(opfuncid);
  WRITE_OID_FIELD(opresulttype);
  WRITE_BOOL_FIELD(opretset);
  WRITE_OID_FIELD(opcollid);
  WRITE_OID_FIELD(inputcollid);
  WRITE_NODE_FIELD(args);
  WRITE_LOCATION_FIELD(location);
}

static void
_outScalarArrayOpExpr(StringInfo str, const ScalarArrayOpExpr *node)
{
  WRITE_NODE_TYPE("SCALARARRAYOPEXPR");

  WRITE_OID_FIELD(opno);
  WRITE_OID_FIELD(opfuncid);
  WRITE_BOOL_FIELD(useOr);
  WRITE_OID_FIELD(inputcollid);
  WRITE_NODE_FIELD(args);
  WRITE_LOCATION_FIELD(location);
}

static void
_outSubLink(StringInfo str, const SubLink *node)
{
  WRITE_NODE_TYPE("SUBLINK");

  WRITE_ENUM_FIELD(subLinkType, SubLinkType);
  WRITE_NODE_FIELD(testexpr);
  WRITE_NODE_FIELD(operName);
  WRITE_NODE_FIELD(subselect);
  WRITE_LOCATION_FIELD(location);
}

static void
_outSubPlan(StringInfo str, const SubPlan *node)
{
  WRITE_NODE_TYPE("SUBPLAN");

  WRITE_ENUM_FIELD(subLinkType, SubLinkType);
  WRITE_NODE_FIELD(testexpr);
  WRITE_NODE_FIELD(paramIds);
  WRITE_INT_FIELD(plan_id);
  WRITE_STRING_FIELD(plan_name);
  WRITE_OID_FIELD(firstColType);
  WRITE_INT_FIELD(firstColTypmod);
  WRITE_OID_FIELD(firstColCollation);
  WRITE_BOOL_FIELD(useHashTable);
  WRITE_BOOL_FIELD(unknownEqFalse);
  WRITE_NODE_FIELD(setParam);
  WRITE_NODE_FIELD(parParam);
  WRITE_NODE_FIELD(args);
  WRITE_FLOAT_FIELD(startup_cost, "%.2f");
  WRITE_FLOAT_FIELD(per_call_cost, "%.2f");
}

static void
_outAlternativeSubPlan(StringInfo str, const AlternativeSubPlan *node)
{
  WRITE_NODE_TYPE("ALTERNATIVESUBPLAN");

  WRITE_NODE_FIELD(subplans);
}

static void
_outFieldSelect(StringInfo str, const FieldSelect *node)
{
  WRITE_NODE_TYPE("FIELDSELECT");

  WRITE_NODE_FIELD(arg);
  WRITE_INT_FIELD(fieldnum);
  WRITE_OID_FIELD(resulttype);
  WRITE_INT_FIELD(resulttypmod);
  WRITE_OID_FIELD(resultcollid);
}

static void
_outFieldStore(StringInfo str, const FieldStore *node)
{
  WRITE_NODE_TYPE("FIELDSTORE");

  WRITE_NODE_FIELD(arg);
  WRITE_NODE_FIELD(newvals);
  WRITE_NODE_FIELD(fieldnums);
  WRITE_OID_FIELD(resulttype);
}

static void
_outRelabelType(StringInfo str, const RelabelType *node)
{
  WRITE_NODE_TYPE("RELABELTYPE");

  WRITE_NODE_FIELD(arg);
  WRITE_OID_FIELD(resulttype);
  WRITE_INT_FIELD(resulttypmod);
  WRITE_OID_FIELD(resultcollid);
  WRITE_ENUM_FIELD(relabelformat, CoercionForm);
  WRITE_LOCATION_FIELD(location);
}

static void
_outCoerceViaIO(StringInfo str, const CoerceViaIO *node)
{
  WRITE_NODE_TYPE("COERCEVIAIO");

  WRITE_NODE_FIELD(arg);
  WRITE_OID_FIELD(resulttype);
  WRITE_OID_FIELD(resultcollid);
  WRITE_ENUM_FIELD(coerceformat, CoercionForm);
  WRITE_LOCATION_FIELD(location);
}

static void
_outArrayCoerceExpr(StringInfo str, const ArrayCoerceExpr *node)
{
  WRITE_NODE_TYPE("ARRAYCOERCEEXPR");

  WRITE_NODE_FIELD(arg);
  WRITE_OID_FIELD(elemfuncid);
  WRITE_OID_FIELD(resulttype);
  WRITE_INT_FIELD(resulttypmod);
  WRITE_OID_FIELD(resultcollid);
  WRITE_BOOL_FIELD(isExplicit);
  WRITE_ENUM_FIELD(coerceformat, CoercionForm);
  WRITE_LOCATION_FIELD(location);
}

static void
_outConvertRowtypeExpr(StringInfo str, const ConvertRowtypeExpr *node)
{
  WRITE_NODE_TYPE("CONVERTROWTYPEEXPR");

  WRITE_NODE_FIELD(arg);
  WRITE_OID_FIELD(resulttype);
  WRITE_ENUM_FIELD(convertformat, CoercionForm);
  WRITE_LOCATION_FIELD(location);
}

static void
_outCollateExpr(StringInfo str, const CollateExpr *node)
{
  WRITE_NODE_TYPE("COLLATE");

  WRITE_NODE_FIELD(arg);
  WRITE_OID_FIELD(collOid);
  WRITE_LOCATION_FIELD(location);
}

static void
_outCaseExpr(StringInfo str, const CaseExpr *node)
{
  WRITE_NODE_TYPE("CASE");

  WRITE_OID_FIELD(casetype);
  WRITE_OID_FIELD(casecollid);
  WRITE_NODE_FIELD(arg);
  WRITE_NODE_FIELD(args);
  WRITE_NODE_FIELD(defresult);
  WRITE_LOCATION_FIELD(location);
}

static void
_outCaseWhen(StringInfo str, const CaseWhen *node)
{
  WRITE_NODE_TYPE("WHEN");

  WRITE_NODE_FIELD(expr);
  WRITE_NODE_FIELD(result);
  WRITE_LOCATION_FIELD(location);
}

static void
_outCaseTestExpr(StringInfo str, const CaseTestExpr *node)
{
  WRITE_NODE_TYPE("CASETESTEXPR");

  WRITE_OID_FIELD(typeId);
  WRITE_INT_FIELD(typeMod);
  WRITE_OID_FIELD(collation);
}

static void
_outArrayExpr(StringInfo str, const ArrayExpr *node)
{
  WRITE_NODE_TYPE("ARRAY");

  WRITE_OID_FIELD(array_typeid);
  WRITE_OID_FIELD(array_collid);
  WRITE_OID_FIELD(element_typeid);
  WRITE_NODE_FIELD(elements);
  WRITE_BOOL_FIELD(multidims);
  WRITE_LOCATION_FIELD(location);
}

static void
_outRowExpr(StringInfo str, const RowExpr *node)
{
  WRITE_NODE_TYPE("ROW");

  WRITE_NODE_FIELD(args);
  WRITE_OID_FIELD(row_typeid);
  WRITE_ENUM_FIELD(row_format, CoercionForm);
  WRITE_NODE_FIELD(colnames);
  WRITE_LOCATION_FIELD(location);
}

static void
_outRowCompareExpr(StringInfo str, const RowCompareExpr *node)
{
  WRITE_NODE_TYPE("ROWCOMPARE");

  WRITE_ENUM_FIELD(rctype, RowCompareType);
  WRITE_NODE_FIELD(opnos);
  WRITE_NODE_FIELD(opfamilies);
  WRITE_NODE_FIELD(inputcollids);
  WRITE_NODE_FIELD(largs);
  WRITE_NODE_FIELD(rargs);
}

static void
_outCoalesceExpr(StringInfo str, const CoalesceExpr *node)
{
  WRITE_NODE_TYPE("COALESCE");

  WRITE_OID_FIELD(coalescetype);
  WRITE_OID_FIELD(coalescecollid);
  WRITE_NODE_FIELD(args);
  WRITE_LOCATION_FIELD(location);
}

static void
_outMinMaxExpr(StringInfo str, const MinMaxExpr *node)
{
  WRITE_NODE_TYPE("MINMAX");

  WRITE_OID_FIELD(minmaxtype);
  WRITE_OID_FIELD(minmaxcollid);
  WRITE_OID_FIELD(inputcollid);
  WRITE_ENUM_FIELD(op, MinMaxOp);
  WRITE_NODE_FIELD(args);
  WRITE_LOCATION_FIELD(location);
}

static void
_outXmlExpr(StringInfo str, const XmlExpr *node)
{
  WRITE_NODE_TYPE("XMLEXPR");

  WRITE_ENUM_FIELD(op, XmlExprOp);
  WRITE_STRING_FIELD(name);
  WRITE_NODE_FIELD(named_args);
  WRITE_NODE_FIELD(arg_names);
  WRITE_NODE_FIELD(args);
  WRITE_ENUM_FIELD(xmloption, XmlOptionType);
  WRITE_OID_FIELD(type);
  WRITE_INT_FIELD(typmod);
  WRITE_LOCATION_FIELD(location);
}

static void
_outNullTest(StringInfo str, const NullTest *node)
{
  WRITE_NODE_TYPE("NULLTEST");

  WRITE_NODE_FIELD(arg);
  WRITE_ENUM_FIELD(nulltesttype, NullTestType);
  WRITE_BOOL_FIELD(argisrow);
}

static void
_outBooleanTest(StringInfo str, const BooleanTest *node)
{
  WRITE_NODE_TYPE("BOOLEANTEST");

  WRITE_NODE_FIELD(arg);
  WRITE_ENUM_FIELD(booltesttype, BoolTestType);
}

static void
_outCoerceToDomain(StringInfo str, const CoerceToDomain *node)
{
  WRITE_NODE_TYPE("COERCETODOMAIN");

  WRITE_NODE_FIELD(arg);
  WRITE_OID_FIELD(resulttype);
  WRITE_INT_FIELD(resulttypmod);
  WRITE_OID_FIELD(resultcollid);
  WRITE_ENUM_FIELD(coercionformat, CoercionForm);
  WRITE_LOCATION_FIELD(location);
}

static void
_outCoerceToDomainValue(StringInfo str, const CoerceToDomainValue *node)
{
  WRITE_NODE_TYPE("COERCETODOMAINVALUE");

  WRITE_OID_FIELD(typeId);
  WRITE_INT_FIELD(typeMod);
  WRITE_OID_FIELD(collation);
  WRITE_LOCATION_FIELD(location);
}

static void
_outSetToDefault(StringInfo str, const SetToDefault *node)
{
  WRITE_NODE_TYPE("SETTODEFAULT");

  WRITE_OID_FIELD(typeId);
  WRITE_INT_FIELD(typeMod);
  WRITE_OID_FIELD(collation);
  WRITE_LOCATION_FIELD(location);
}

static void
_outCurrentOfExpr(StringInfo str, const CurrentOfExpr *node)
{
  WRITE_NODE_TYPE("CURRENTOFEXPR");

  WRITE_UINT_FIELD(cvarno);
  WRITE_STRING_FIELD(cursor_name);
  WRITE_INT_FIELD(cursor_param);
}

static void
_outTargetEntry(StringInfo str, const TargetEntry *node)
{
  WRITE_NODE_TYPE("TARGETENTRY");

  WRITE_NODE_FIELD(expr);
  WRITE_INT_FIELD(resno);
  WRITE_STRING_FIELD(resname);
  WRITE_UINT_FIELD(ressortgroupref);
  WRITE_OID_FIELD(resorigtbl);
  WRITE_INT_FIELD(resorigcol);
  WRITE_BOOL_FIELD(resjunk);
}

static void
_outRangeTblRef(StringInfo str, const RangeTblRef *node)
{
  WRITE_NODE_TYPE("RANGETBLREF");

  WRITE_INT_FIELD(rtindex);
}

static void
_outJoinExpr(StringInfo str, const JoinExpr *node)
{
  WRITE_NODE_TYPE("JOINEXPR");

  WRITE_ENUM_FIELD(jointype, JoinType);
  WRITE_BOOL_FIELD(isNatural);
  WRITE_NODE_FIELD(larg);
  WRITE_NODE_FIELD(rarg);
  WRITE_NODE_FIELD(usingClause);
  WRITE_NODE_FIELD(quals);
  WRITE_NODE_FIELD(alias);
  WRITE_INT_FIELD(rtindex);
}

static void
_outFromExpr(StringInfo str, const FromExpr *node)
{
  WRITE_NODE_TYPE("FROMEXPR");

  WRITE_NODE_FIELD(fromlist);
  WRITE_NODE_FIELD(quals);
}

static void
_outIntoClause(StringInfo str, const IntoClause *node)
{
  WRITE_NODE_TYPE("INTOCLAUSE");

  WRITE_NODE_FIELD(rel);
  WRITE_NODE_FIELD(colNames);
  WRITE_NODE_FIELD(options);
  WRITE_ENUM_FIELD(onCommit, OnCommitAction);
  WRITE_STRING_FIELD(tableSpaceName);
  WRITE_NODE_FIELD(viewQuery);
  WRITE_BOOL_FIELD(skipData);
}

static void
_outPlannerInfo(StringInfo str, const PlannerInfo *node)
{
  WRITE_NODE_TYPE("PLANNERINFO");

  WRITE_NODE_FIELD(parse);
  WRITE_NODE_FIELD(glob);
  WRITE_UINT_FIELD(query_level);
  WRITE_NODE_FIELD(plan_params);
  WRITE_BITMAPSET_FIELD(all_baserels);
  WRITE_BITMAPSET_FIELD(nullable_baserels);
  WRITE_NODE_FIELD(join_rel_list);
  WRITE_INT_FIELD(join_cur_level);
  WRITE_NODE_FIELD(init_plans);
  WRITE_NODE_FIELD(cte_plan_ids);
  WRITE_NODE_FIELD(eq_classes);
  WRITE_NODE_FIELD(canon_pathkeys);
  WRITE_NODE_FIELD(left_join_clauses);
  WRITE_NODE_FIELD(right_join_clauses);
  WRITE_NODE_FIELD(full_join_clauses);
  WRITE_NODE_FIELD(join_info_list);
  WRITE_NODE_FIELD(lateral_info_list);
  WRITE_NODE_FIELD(append_rel_list);
  WRITE_NODE_FIELD(rowMarks);
  WRITE_NODE_FIELD(placeholder_list);
  WRITE_NODE_FIELD(query_pathkeys);
  WRITE_NODE_FIELD(group_pathkeys);
  WRITE_NODE_FIELD(window_pathkeys);
  WRITE_NODE_FIELD(distinct_pathkeys);
  WRITE_NODE_FIELD(sort_pathkeys);
  WRITE_NODE_FIELD(minmax_aggs);
  WRITE_FLOAT_FIELD(total_table_pages, "%.0f");
  WRITE_FLOAT_FIELD(tuple_fraction, "%.4f");
  WRITE_FLOAT_FIELD(limit_tuples, "%.0f");
  WRITE_BOOL_FIELD(hasInheritedTarget);
  WRITE_BOOL_FIELD(hasJoinRTEs);
  WRITE_BOOL_FIELD(hasLateralRTEs);
  WRITE_BOOL_FIELD(hasHavingQual);
  WRITE_BOOL_FIELD(hasPseudoConstantQuals);
  WRITE_BOOL_FIELD(hasRecursion);
  WRITE_INT_FIELD(wt_param_id);
  WRITE_BITMAPSET_FIELD(curOuterRels);
  WRITE_NODE_FIELD(curOuterParams);
}

static void
_outPlannerGlobal(StringInfo str, const PlannerGlobal *node)
{
  WRITE_NODE_TYPE("PLANNERGLOBAL");

  WRITE_NODE_FIELD(subplans);
  WRITE_BITMAPSET_FIELD(rewindPlanIDs);
  WRITE_NODE_FIELD(finalrtable);
  WRITE_NODE_FIELD(finalrowmarks);
  WRITE_NODE_FIELD(resultRelations);
  WRITE_NODE_FIELD(relationOids);
  WRITE_NODE_FIELD(invalItems);
  WRITE_INT_FIELD(nParamExec);
  WRITE_UINT_FIELD(lastPHId);
  WRITE_UINT_FIELD(lastRowMarkId);
  WRITE_BOOL_FIELD(transientPlan);
}

static void
_outRelOptInfo(StringInfo str, const RelOptInfo *node)
{
  WRITE_NODE_TYPE("RELOPTINFO");

  WRITE_ENUM_FIELD(reloptkind, RelOptKind);
  WRITE_BITMAPSET_FIELD(relids);
  WRITE_FLOAT_FIELD(rows, "%.0f");
  WRITE_INT_FIELD(width);
  WRITE_BOOL_FIELD(consider_startup);
  WRITE_NODE_FIELD(reltargetlist);
  WRITE_NODE_FIELD(pathlist);
  WRITE_NODE_FIELD(ppilist);
  WRITE_NODE_FIELD(cheapest_startup_path);
  WRITE_NODE_FIELD(cheapest_total_path);
  WRITE_NODE_FIELD(cheapest_unique_path);
  WRITE_NODE_FIELD(cheapest_parameterized_paths);
  WRITE_UINT_FIELD(relid);
  WRITE_UINT_FIELD(reltablespace);
  WRITE_ENUM_FIELD(rtekind, RTEKind);
  WRITE_INT_FIELD(min_attr);
  WRITE_INT_FIELD(max_attr);
  WRITE_NODE_FIELD(lateral_vars);
  WRITE_BITMAPSET_FIELD(lateral_relids);
  WRITE_BITMAPSET_FIELD(lateral_referencers);
  WRITE_NODE_FIELD(indexlist);
  WRITE_UINT_FIELD(pages);
  WRITE_FLOAT_FIELD(tuples, "%.0f");
  WRITE_FLOAT_FIELD(allvisfrac, "%.6f");
  WRITE_NODE_FIELD(subplan);
  WRITE_NODE_FIELD(subroot);
  WRITE_NODE_FIELD(subplan_params);
  WRITE_NODE_FIELD(baserestrictinfo);
  WRITE_NODE_FIELD(joininfo);
  WRITE_BOOL_FIELD(has_eclass_joins);
}

static void
_outIndexOptInfo(StringInfo str, const IndexOptInfo *node)
{
  WRITE_NODE_TYPE("INDEXOPTINFO");

  WRITE_OID_FIELD(indexoid);
  WRITE_UINT_FIELD(pages);
  WRITE_FLOAT_FIELD(tuples, "%.0f");
  WRITE_INT_FIELD(tree_height);
  WRITE_INT_FIELD(ncolumns);
  WRITE_OID_FIELD(relam);
  WRITE_NODE_FIELD(indpred);
  WRITE_NODE_FIELD(indextlist);
  WRITE_BOOL_FIELD(predOK);
  WRITE_BOOL_FIELD(unique);
  WRITE_BOOL_FIELD(immediate);
  WRITE_BOOL_FIELD(hypothetical);
}

static void
_outParamPathInfo(StringInfo str, const ParamPathInfo *node)
{
  WRITE_NODE_TYPE("PARAMPATHINFO");

  WRITE_BITMAPSET_FIELD(ppi_req_outer);
  WRITE_FLOAT_FIELD(ppi_rows, "%.0f");
  WRITE_NODE_FIELD(ppi_clauses);
}

static void
_outIndexPath(StringInfo str, const IndexPath *node)
{
  WRITE_NODE_TYPE("INDEXPATH");

  _outPathInfo(str, (const Path *) node);

  WRITE_NODE_FIELD(indexinfo);
  WRITE_NODE_FIELD(indexclauses);
  WRITE_NODE_FIELD(indexquals);
  WRITE_NODE_FIELD(indexqualcols);
  WRITE_NODE_FIELD(indexorderbys);
  WRITE_NODE_FIELD(indexorderbycols);
  WRITE_ENUM_FIELD(indexscandir, ScanDirection);
  WRITE_FLOAT_FIELD(indextotalcost, "%.2f");
  WRITE_FLOAT_FIELD(indexselectivity, "%.4f");
}

static void
_outBitmapHeapPath(StringInfo str, const BitmapHeapPath *node)
{
  WRITE_NODE_TYPE("BITMAPHEAPPATH");

  _outPathInfo(str, (const Path *) node);

  WRITE_NODE_FIELD(bitmapqual);
}

static void
_outBitmapAndPath(StringInfo str, const BitmapAndPath *node)
{
  WRITE_NODE_TYPE("BITMAPANDPATH");

  _outPathInfo(str, (const Path *) node);

  WRITE_NODE_FIELD(bitmapquals);
  WRITE_FLOAT_FIELD(bitmapselectivity, "%.4f");
}

static void
_outBitmapOrPath(StringInfo str, const BitmapOrPath *node)
{
  WRITE_NODE_TYPE("BITMAPORPATH");

  _outPathInfo(str, (const Path *) node);

  WRITE_NODE_FIELD(bitmapquals);
  WRITE_FLOAT_FIELD(bitmapselectivity, "%.4f");
}

static void
_outNestPath(StringInfo str, const NestPath *node)
{
  WRITE_NODE_TYPE("NESTPATH");

  _outJoinPathInfo(str, (const JoinPath *) node);

}

static void
_outMergePath(StringInfo str, const MergePath *node)
{
  WRITE_NODE_TYPE("MERGEPATH");

  _outJoinPathInfo(str, (const JoinPath *) node);

  WRITE_NODE_FIELD(path_mergeclauses);
  WRITE_NODE_FIELD(outersortkeys);
  WRITE_NODE_FIELD(innersortkeys);
  WRITE_BOOL_FIELD(materialize_inner);
}

static void
_outHashPath(StringInfo str, const HashPath *node)
{
  WRITE_NODE_TYPE("HASHPATH");

  _outJoinPathInfo(str, (const JoinPath *) node);

  WRITE_NODE_FIELD(path_hashclauses);
  WRITE_INT_FIELD(num_batches);
}

static void
_outTidPath(StringInfo str, const TidPath *node)
{
  WRITE_NODE_TYPE("TIDPATH");

  _outPathInfo(str, (const Path *) node);

  WRITE_NODE_FIELD(tidquals);
}

static void
_outForeignPath(StringInfo str, const ForeignPath *node)
{
  WRITE_NODE_TYPE("FOREIGNPATH");

  _outPathInfo(str, (const Path *) node);

  WRITE_NODE_FIELD(fdw_private);
}

static void
_outAppendPath(StringInfo str, const AppendPath *node)
{
  WRITE_NODE_TYPE("APPENDPATH");

  _outPathInfo(str, (const Path *) node);

  WRITE_NODE_FIELD(subpaths);
}

static void
_outMergeAppendPath(StringInfo str, const MergeAppendPath *node)
{
  WRITE_NODE_TYPE("MERGEAPPENDPATH");

  _outPathInfo(str, (const Path *) node);

  WRITE_NODE_FIELD(subpaths);
  WRITE_FLOAT_FIELD(limit_tuples, "%.0f");
}

static void
_outResultPath(StringInfo str, const ResultPath *node)
{
  WRITE_NODE_TYPE("RESULTPATH");

  _outPathInfo(str, (const Path *) node);

  WRITE_NODE_FIELD(quals);
}

static void
_outMaterialPath(StringInfo str, const MaterialPath *node)
{
  WRITE_NODE_TYPE("MATERIALPATH");

  _outPathInfo(str, (const Path *) node);

  WRITE_NODE_FIELD(subpath);
}

static void
_outUniquePath(StringInfo str, const UniquePath *node)
{
  WRITE_NODE_TYPE("UNIQUEPATH");

  _outPathInfo(str, (const Path *) node);

  WRITE_NODE_FIELD(subpath);
  WRITE_ENUM_FIELD(umethod, UniquePathMethod);
  WRITE_NODE_FIELD(in_operators);
  WRITE_NODE_FIELD(uniq_exprs);
}

static void
_outEquivalenceMember(StringInfo str, const EquivalenceMember *node)
{
  WRITE_NODE_TYPE("EQUIVALENCEMEMBER");

  WRITE_NODE_FIELD(em_expr);
  WRITE_BITMAPSET_FIELD(em_relids);
  WRITE_BITMAPSET_FIELD(em_nullable_relids);
  WRITE_BOOL_FIELD(em_is_const);
  WRITE_BOOL_FIELD(em_is_child);
  WRITE_OID_FIELD(em_datatype);
}

static void
_outPathKey(StringInfo str, const PathKey *node)
{
  WRITE_NODE_TYPE("PATHKEY");

  WRITE_NODE_FIELD(pk_eclass);
  WRITE_OID_FIELD(pk_opfamily);
  WRITE_INT_FIELD(pk_strategy);
  WRITE_BOOL_FIELD(pk_nulls_first);
}

static void
_outRestrictInfo(StringInfo str, const RestrictInfo *node)
{
  WRITE_NODE_TYPE("RESTRICTINFO");

  WRITE_NODE_FIELD(clause);
  WRITE_BOOL_FIELD(is_pushed_down);
  WRITE_BOOL_FIELD(outerjoin_delayed);
  WRITE_BOOL_FIELD(can_join);
  WRITE_BOOL_FIELD(pseudoconstant);
  WRITE_BITMAPSET_FIELD(clause_relids);
  WRITE_BITMAPSET_FIELD(required_relids);
  WRITE_BITMAPSET_FIELD(outer_relids);
  WRITE_BITMAPSET_FIELD(nullable_relids);
  WRITE_BITMAPSET_FIELD(left_relids);
  WRITE_BITMAPSET_FIELD(right_relids);
  WRITE_NODE_FIELD(orclause);
  WRITE_FLOAT_FIELD(norm_selec, "%.4f");
  WRITE_FLOAT_FIELD(outer_selec, "%.4f");
  WRITE_NODE_FIELD(mergeopfamilies);
  WRITE_NODE_FIELD(left_em);
  WRITE_NODE_FIELD(right_em);
  WRITE_BOOL_FIELD(outer_is_left);
  WRITE_OID_FIELD(hashjoinoperator);
}

static void
_outPlaceHolderVar(StringInfo str, const PlaceHolderVar *node)
{
  WRITE_NODE_TYPE("PLACEHOLDERVAR");

  WRITE_NODE_FIELD(phexpr);
  WRITE_BITMAPSET_FIELD(phrels);
  WRITE_UINT_FIELD(phid);
  WRITE_UINT_FIELD(phlevelsup);
}

static void
_outSpecialJoinInfo(StringInfo str, const SpecialJoinInfo *node)
{
  WRITE_NODE_TYPE("SPECIALJOININFO");

  WRITE_BITMAPSET_FIELD(min_lefthand);
  WRITE_BITMAPSET_FIELD(min_righthand);
  WRITE_BITMAPSET_FIELD(syn_lefthand);
  WRITE_BITMAPSET_FIELD(syn_righthand);
  WRITE_ENUM_FIELD(jointype, JoinType);
  WRITE_BOOL_FIELD(lhs_strict);
  WRITE_BOOL_FIELD(delay_upper_joins);
  WRITE_NODE_FIELD(join_quals);
}

static void
_outLateralJoinInfo(StringInfo str, const LateralJoinInfo *node)
{
  WRITE_NODE_TYPE("LATERALJOININFO");

  WRITE_BITMAPSET_FIELD(lateral_lhs);
  WRITE_BITMAPSET_FIELD(lateral_rhs);
}

static void
_outAppendRelInfo(StringInfo str, const AppendRelInfo *node)
{
  WRITE_NODE_TYPE("APPENDRELINFO");

  WRITE_UINT_FIELD(parent_relid);
  WRITE_UINT_FIELD(child_relid);
  WRITE_OID_FIELD(parent_reltype);
  WRITE_OID_FIELD(child_reltype);
  WRITE_NODE_FIELD(translated_vars);
  WRITE_OID_FIELD(parent_reloid);
}

static void
_outPlaceHolderInfo(StringInfo str, const PlaceHolderInfo *node)
{
  WRITE_NODE_TYPE("PLACEHOLDERINFO");

  WRITE_UINT_FIELD(phid);
  WRITE_NODE_FIELD(ph_var);
  WRITE_BITMAPSET_FIELD(ph_eval_at);
  WRITE_BITMAPSET_FIELD(ph_lateral);
  WRITE_BITMAPSET_FIELD(ph_needed);
  WRITE_INT_FIELD(ph_width);
}

static void
_outMinMaxAggInfo(StringInfo str, const MinMaxAggInfo *node)
{
  WRITE_NODE_TYPE("MINMAXAGGINFO");

  WRITE_OID_FIELD(aggfnoid);
  WRITE_OID_FIELD(aggsortop);
  WRITE_NODE_FIELD(target);
  WRITE_NODE_FIELD(path);
  WRITE_FLOAT_FIELD(pathcost, "%.2f");
  WRITE_NODE_FIELD(param);
}

static void
_outPlannerParamItem(StringInfo str, const PlannerParamItem *node)
{
  WRITE_NODE_TYPE("PLANNERPARAMITEM");

  WRITE_NODE_FIELD(item);
  WRITE_INT_FIELD(paramId);
}

static void
_outQuery(StringInfo str, const Query *node)
{
  WRITE_NODE_TYPE("QUERY");

  WRITE_ENUM_FIELD(commandType, CmdType);
  WRITE_ENUM_FIELD(querySource, QuerySource);
  WRITE_BOOL_FIELD(canSetTag);
  WRITE_NODE_FIELD(utilityStmt);
  WRITE_INT_FIELD(resultRelation);
  WRITE_BOOL_FIELD(hasAggs);
  WRITE_BOOL_FIELD(hasWindowFuncs);
  WRITE_BOOL_FIELD(hasSubLinks);
  WRITE_BOOL_FIELD(hasDistinctOn);
  WRITE_BOOL_FIELD(hasRecursive);
  WRITE_BOOL_FIELD(hasModifyingCTE);
  WRITE_BOOL_FIELD(hasForUpdate);
  WRITE_NODE_FIELD(cteList);
  WRITE_NODE_FIELD(rtable);
  WRITE_NODE_FIELD(jointree);
  WRITE_NODE_FIELD(targetList);
  WRITE_NODE_FIELD(withCheckOptions);
  WRITE_NODE_FIELD(returningList);
  WRITE_NODE_FIELD(groupClause);
  WRITE_NODE_FIELD(havingQual);
  WRITE_NODE_FIELD(windowClause);
  WRITE_NODE_FIELD(distinctClause);
  WRITE_NODE_FIELD(sortClause);
  WRITE_NODE_FIELD(limitOffset);
  WRITE_NODE_FIELD(limitCount);
  WRITE_NODE_FIELD(rowMarks);
  WRITE_NODE_FIELD(setOperations);
  WRITE_NODE_FIELD(constraintDeps);
}

static void
_outPlannedStmt(StringInfo str, const PlannedStmt *node)
{
  WRITE_NODE_TYPE("PLANNEDSTMT");

  WRITE_ENUM_FIELD(commandType, CmdType);
  WRITE_UINT_FIELD(queryId);
  WRITE_BOOL_FIELD(hasReturning);
  WRITE_BOOL_FIELD(hasModifyingCTE);
  WRITE_BOOL_FIELD(canSetTag);
  WRITE_BOOL_FIELD(transientPlan);
  WRITE_NODE_FIELD(planTree);
  WRITE_NODE_FIELD(rtable);
  WRITE_NODE_FIELD(resultRelations);
  WRITE_NODE_FIELD(utilityStmt);
  WRITE_NODE_FIELD(subplans);
  WRITE_BITMAPSET_FIELD(rewindPlanIDs);
  WRITE_NODE_FIELD(rowMarks);
  WRITE_NODE_FIELD(relationOids);
  WRITE_NODE_FIELD(invalItems);
  WRITE_INT_FIELD(nParamExec);
}

static void
_outInsertStmt(StringInfo str, const InsertStmt *node)
{
  WRITE_NODE_TYPE("INSERT INTO");

  WRITE_NODE_FIELD(relation);
  WRITE_NODE_FIELD(cols);
  WRITE_NODE_FIELD(selectStmt);
  WRITE_NODE_FIELD(returningList);
  WRITE_NODE_FIELD(withClause);
}

static void
_outDeleteStmt(StringInfo str, const DeleteStmt *node)
{
  WRITE_NODE_TYPE("DELETE FROM");

  WRITE_NODE_FIELD(relation);
  WRITE_NODE_FIELD(usingClause);
  WRITE_NODE_FIELD(whereClause);
  WRITE_NODE_FIELD(returningList);
  WRITE_NODE_FIELD(withClause);
}

static void
_outUpdateStmt(StringInfo str, const UpdateStmt *node)
{
  WRITE_NODE_TYPE("UPDATE");

  WRITE_NODE_FIELD(relation);
  WRITE_NODE_FIELD(targetList);
  WRITE_NODE_FIELD(whereClause);
  WRITE_NODE_FIELD(fromClause);
  WRITE_NODE_FIELD(returningList);
  WRITE_NODE_FIELD(withClause);
}

static void
_outSelectStmt(StringInfo str, const SelectStmt *node)
{
  WRITE_NODE_TYPE("SELECT");

  WRITE_NODE_FIELD(distinctClause);
  WRITE_NODE_FIELD(intoClause);
  WRITE_NODE_FIELD(targetList);
  WRITE_NODE_FIELD(fromClause);
  WRITE_NODE_FIELD(whereClause);
  WRITE_NODE_FIELD(groupClause);
  WRITE_NODE_FIELD(havingClause);
  WRITE_NODE_FIELD(windowClause);
  WRITE_NODE_FIELD(valuesLists);
  WRITE_NODE_FIELD(sortClause);
  WRITE_NODE_FIELD(limitOffset);
  WRITE_NODE_FIELD(limitCount);
  WRITE_NODE_FIELD(lockingClause);
  WRITE_NODE_FIELD(withClause);
  WRITE_ENUM_FIELD(op, SetOperation);
  WRITE_BOOL_FIELD(all);
  WRITE_NODE_FIELD(larg);
  WRITE_NODE_FIELD(rarg);
}

static void
_outAlterTableStmt(StringInfo str, const AlterTableStmt *node)
{
  WRITE_NODE_TYPE("ALTER TABLE");

  WRITE_NODE_FIELD(relation);
  WRITE_NODE_FIELD(cmds);
  WRITE_ENUM_FIELD(relkind, ObjectType);
  WRITE_BOOL_FIELD(missing_ok);
}

static void
_outAlterTableCmd(StringInfo str, const AlterTableCmd *node)
{
  WRITE_NODE_TYPE("ALTER TABLE CMD");

  WRITE_ENUM_FIELD(subtype, AlterTableType);
  WRITE_STRING_FIELD(name);
  WRITE_NODE_FIELD(def);
  WRITE_ENUM_FIELD(behavior, DropBehavior);
  WRITE_BOOL_FIELD(missing_ok);
}

static void
_outAlterDomainStmt(StringInfo str, const AlterDomainStmt *node)
{
  WRITE_NODE_TYPE("ALTERDOMAINSTMT");

  WRITE_CHAR_FIELD(subtype);
  WRITE_NODE_FIELD(typeName);
  WRITE_STRING_FIELD(name);
  WRITE_NODE_FIELD(def);
  WRITE_ENUM_FIELD(behavior, DropBehavior);
  WRITE_BOOL_FIELD(missing_ok);
}

static void
_outSetOperationStmt(StringInfo str, const SetOperationStmt *node)
{
  WRITE_NODE_TYPE("SETOPERATIONSTMT");

  WRITE_ENUM_FIELD(op, SetOperation);
  WRITE_BOOL_FIELD(all);
  WRITE_NODE_FIELD(larg);
  WRITE_NODE_FIELD(rarg);
  WRITE_NODE_FIELD(colTypes);
  WRITE_NODE_FIELD(colTypmods);
  WRITE_NODE_FIELD(colCollations);
  WRITE_NODE_FIELD(groupClauses);
}

static void
_outGrantStmt(StringInfo str, const GrantStmt *node)
{
  WRITE_NODE_TYPE("GRANTSTMT");

  WRITE_BOOL_FIELD(is_grant);
  WRITE_ENUM_FIELD(targtype, GrantTargetType);
  WRITE_ENUM_FIELD(objtype, GrantObjectType);
  WRITE_NODE_FIELD(objects);
  WRITE_NODE_FIELD(privileges);
  WRITE_NODE_FIELD(grantees);
  WRITE_BOOL_FIELD(grant_option);
  WRITE_ENUM_FIELD(behavior, DropBehavior);
}

static void
_outGrantRoleStmt(StringInfo str, const GrantRoleStmt *node)
{
  WRITE_NODE_TYPE("GRANTROLESTMT");

  WRITE_NODE_FIELD(granted_roles);
  WRITE_NODE_FIELD(grantee_roles);
  WRITE_BOOL_FIELD(is_grant);
  WRITE_BOOL_FIELD(admin_opt);
  WRITE_STRING_FIELD(grantor);
  WRITE_ENUM_FIELD(behavior, DropBehavior);
}

static void
_outAlterDefaultPrivilegesStmt(StringInfo str, const AlterDefaultPrivilegesStmt *node)
{
  WRITE_NODE_TYPE("ALTERDEFAULTPRIVILEGESSTMT");

  WRITE_NODE_FIELD(options);
  WRITE_NODE_FIELD(action);
}

static void
_outClosePortalStmt(StringInfo str, const ClosePortalStmt *node)
{
  WRITE_NODE_TYPE("CLOSEPORTALSTMT");

  WRITE_STRING_FIELD(portalname);
}

static void
_outClusterStmt(StringInfo str, const ClusterStmt *node)
{
  WRITE_NODE_TYPE("CLUSTERSTMT");

  WRITE_NODE_FIELD(relation);
  WRITE_STRING_FIELD(indexname);
  WRITE_BOOL_FIELD(verbose);
}

static void
_outCopyStmt(StringInfo str, const CopyStmt *node)
{
  WRITE_NODE_TYPE("COPY");

  WRITE_NODE_FIELD(relation);
  WRITE_NODE_FIELD(query);
  WRITE_NODE_FIELD(attlist);
  WRITE_BOOL_FIELD(is_from);
  WRITE_BOOL_FIELD(is_program);
  WRITE_STRING_FIELD(filename);
  WRITE_NODE_FIELD(options);
}

static void
_outCreateStmt(StringInfo str, const CreateStmt *node)
{
  WRITE_NODE_TYPE("CREATESTMT");

  _outCreateStmtInfo(str, (const CreateStmt *) node);

}

static void
_outDefineStmt(StringInfo str, const DefineStmt *node)
{
  WRITE_NODE_TYPE("DEFINESTMT");

  WRITE_ENUM_FIELD(kind, ObjectType);
  WRITE_BOOL_FIELD(oldstyle);
  WRITE_NODE_FIELD(defnames);
  WRITE_NODE_FIELD(args);
  WRITE_NODE_FIELD(definition);
}

static void
_outDropStmt(StringInfo str, const DropStmt *node)
{
  WRITE_NODE_TYPE("DROP");

  WRITE_NODE_FIELD(objects);
  WRITE_NODE_FIELD(arguments);
  WRITE_ENUM_FIELD(removeType, ObjectType);
  WRITE_ENUM_FIELD(behavior, DropBehavior);
  WRITE_BOOL_FIELD(missing_ok);
  WRITE_BOOL_FIELD(concurrent);
}

static void
_outTruncateStmt(StringInfo str, const TruncateStmt *node)
{
  WRITE_NODE_TYPE("TRUNCATE");

  WRITE_NODE_FIELD(relations);
  WRITE_BOOL_FIELD(restart_seqs);
  WRITE_ENUM_FIELD(behavior, DropBehavior);
}

static void
_outCommentStmt(StringInfo str, const CommentStmt *node)
{
  WRITE_NODE_TYPE("COMMENTSTMT");

  WRITE_ENUM_FIELD(objtype, ObjectType);
  WRITE_NODE_FIELD(objname);
  WRITE_NODE_FIELD(objargs);
  WRITE_STRING_FIELD(comment);
}

static void
_outFetchStmt(StringInfo str, const FetchStmt *node)
{
  WRITE_NODE_TYPE("FETCHSTMT");

  WRITE_ENUM_FIELD(direction, FetchDirection);
  WRITE_LONG_FIELD(howMany);
  WRITE_STRING_FIELD(portalname);
  WRITE_BOOL_FIELD(ismove);
}

static void
_outIndexStmt(StringInfo str, const IndexStmt *node)
{
  WRITE_NODE_TYPE("INDEXSTMT");

  WRITE_STRING_FIELD(idxname);
  WRITE_NODE_FIELD(relation);
  WRITE_STRING_FIELD(accessMethod);
  WRITE_STRING_FIELD(tableSpace);
  WRITE_NODE_FIELD(indexParams);
  WRITE_NODE_FIELD(options);
  WRITE_NODE_FIELD(whereClause);
  WRITE_NODE_FIELD(excludeOpNames);
  WRITE_STRING_FIELD(idxcomment);
  WRITE_OID_FIELD(indexOid);
  WRITE_OID_FIELD(oldNode);
  WRITE_BOOL_FIELD(unique);
  WRITE_BOOL_FIELD(primary);
  WRITE_BOOL_FIELD(isconstraint);
  WRITE_BOOL_FIELD(deferrable);
  WRITE_BOOL_FIELD(initdeferred);
  WRITE_BOOL_FIELD(concurrent);
}

static void
_outCreateFunctionStmt(StringInfo str, const CreateFunctionStmt *node)
{
  WRITE_NODE_TYPE("CREATEFUNCTIONSTMT");

  WRITE_BOOL_FIELD(replace);
  WRITE_NODE_FIELD(funcname);
  WRITE_NODE_FIELD(parameters);
  WRITE_NODE_FIELD(returnType);
  WRITE_NODE_FIELD(options);
  WRITE_NODE_FIELD(withClause);
}

static void
_outAlterFunctionStmt(StringInfo str, const AlterFunctionStmt *node)
{
  WRITE_NODE_TYPE("ALTERFUNCTIONSTMT");

  WRITE_NODE_FIELD(func);
  WRITE_NODE_FIELD(actions);
}

static void
_outDoStmt(StringInfo str, const DoStmt *node)
{
  WRITE_NODE_TYPE("DOSTMT");

  WRITE_NODE_FIELD(args);
}

static void
_outRenameStmt(StringInfo str, const RenameStmt *node)
{
  WRITE_NODE_TYPE("RENAMESTMT");

  WRITE_ENUM_FIELD(renameType, ObjectType);
  WRITE_ENUM_FIELD(relationType, ObjectType);
  WRITE_NODE_FIELD(relation);
  WRITE_NODE_FIELD(object);
  WRITE_NODE_FIELD(objarg);
  WRITE_STRING_FIELD(subname);
  WRITE_STRING_FIELD(newname);
  WRITE_ENUM_FIELD(behavior, DropBehavior);
  WRITE_BOOL_FIELD(missing_ok);
}

static void
_outRuleStmt(StringInfo str, const RuleStmt *node)
{
  WRITE_NODE_TYPE("RULESTMT");

  WRITE_NODE_FIELD(relation);
  WRITE_STRING_FIELD(rulename);
  WRITE_NODE_FIELD(whereClause);
  WRITE_ENUM_FIELD(event, CmdType);
  WRITE_BOOL_FIELD(instead);
  WRITE_NODE_FIELD(actions);
  WRITE_BOOL_FIELD(replace);
}

static void
_outNotifyStmt(StringInfo str, const NotifyStmt *node)
{
  WRITE_NODE_TYPE("NOTIFYSTMT");

  WRITE_STRING_FIELD(conditionname);
  WRITE_STRING_FIELD(payload);
}

static void
_outListenStmt(StringInfo str, const ListenStmt *node)
{
  WRITE_NODE_TYPE("LISTENSTMT");

  WRITE_STRING_FIELD(conditionname);
}

static void
_outUnlistenStmt(StringInfo str, const UnlistenStmt *node)
{
  WRITE_NODE_TYPE("UNLISTENSTMT");

  WRITE_STRING_FIELD(conditionname);
}

static void
_outTransactionStmt(StringInfo str, const TransactionStmt *node)
{
  WRITE_NODE_TYPE("TRANSACTION");

  WRITE_ENUM_FIELD(kind, TransactionStmtKind);
  WRITE_NODE_FIELD(options);
  WRITE_STRING_FIELD(gid);
}

static void
_outViewStmt(StringInfo str, const ViewStmt *node)
{
  WRITE_NODE_TYPE("VIEWSTMT");

  WRITE_NODE_FIELD(view);
  WRITE_NODE_FIELD(aliases);
  WRITE_NODE_FIELD(query);
  WRITE_BOOL_FIELD(replace);
  WRITE_NODE_FIELD(options);
  WRITE_ENUM_FIELD(withCheckOption, ViewCheckOption);
}

static void
_outLoadStmt(StringInfo str, const LoadStmt *node)
{
  WRITE_NODE_TYPE("LOADSTMT");

  WRITE_STRING_FIELD(filename);
}

static void
_outCreateDomainStmt(StringInfo str, const CreateDomainStmt *node)
{
  WRITE_NODE_TYPE("CREATEDOMAINSTMT");

  WRITE_NODE_FIELD(domainname);
  WRITE_NODE_FIELD(typeName);
  WRITE_NODE_FIELD(collClause);
  WRITE_NODE_FIELD(constraints);
}

static void
_outCreatedbStmt(StringInfo str, const CreatedbStmt *node)
{
  WRITE_NODE_TYPE("CREATEDBSTMT");

  WRITE_STRING_FIELD(dbname);
  WRITE_NODE_FIELD(options);
}

static void
_outDropdbStmt(StringInfo str, const DropdbStmt *node)
{
  WRITE_NODE_TYPE("DROPDBSTMT");

  WRITE_STRING_FIELD(dbname);
  WRITE_BOOL_FIELD(missing_ok);
}

static void
_outVacuumStmt(StringInfo str, const VacuumStmt *node)
{
  WRITE_NODE_TYPE("VACUUM");

  WRITE_ENUM_FIELD(options, VacuumOption);
  WRITE_INT_FIELD(freeze_min_age);
  WRITE_INT_FIELD(freeze_table_age);
  WRITE_INT_FIELD(multixact_freeze_min_age);
  WRITE_INT_FIELD(multixact_freeze_table_age);
  WRITE_NODE_FIELD(relation);
  WRITE_NODE_FIELD(va_cols);
}

static void
_outExplainStmt(StringInfo str, const ExplainStmt *node)
{
  WRITE_NODE_TYPE("EXPLAIN");

  WRITE_NODE_FIELD(query);
  WRITE_NODE_FIELD(options);
}

static void
_outCreateTableAsStmt(StringInfo str, const CreateTableAsStmt *node)
{
  WRITE_NODE_TYPE("CREATE TABLE AS");

  WRITE_NODE_FIELD(query);
  WRITE_NODE_FIELD(into);
  WRITE_ENUM_FIELD(relkind, ObjectType);
  WRITE_BOOL_FIELD(is_select_into);
}

static void
_outCreateSeqStmt(StringInfo str, const CreateSeqStmt *node)
{
  WRITE_NODE_TYPE("CREATESEQSTMT");

  WRITE_NODE_FIELD(sequence);
  WRITE_NODE_FIELD(options);
  WRITE_OID_FIELD(ownerId);
}

static void
_outAlterSeqStmt(StringInfo str, const AlterSeqStmt *node)
{
  WRITE_NODE_TYPE("ALTERSEQSTMT");

  WRITE_NODE_FIELD(sequence);
  WRITE_NODE_FIELD(options);
  WRITE_BOOL_FIELD(missing_ok);
}

static void
_outVariableSetStmt(StringInfo str, const VariableSetStmt *node)
{
  WRITE_NODE_TYPE("SET");

  WRITE_ENUM_FIELD(kind, VariableSetKind);
  WRITE_STRING_FIELD(name);
  WRITE_NODE_FIELD(args);
  WRITE_BOOL_FIELD(is_local);
}

static void
_outVariableShowStmt(StringInfo str, const VariableShowStmt *node)
{
  WRITE_NODE_TYPE("SHOW");

  WRITE_STRING_FIELD(name);
}

static void
_outDiscardStmt(StringInfo str, const DiscardStmt *node)
{
  WRITE_NODE_TYPE("DISCARDSTMT");

  WRITE_ENUM_FIELD(target, DiscardMode);
}

static void
_outCreateTrigStmt(StringInfo str, const CreateTrigStmt *node)
{
  WRITE_NODE_TYPE("CREATETRIGSTMT");

  WRITE_STRING_FIELD(trigname);
  WRITE_NODE_FIELD(relation);
  WRITE_NODE_FIELD(funcname);
  WRITE_NODE_FIELD(args);
  WRITE_BOOL_FIELD(row);
  WRITE_INT_FIELD(timing);
  WRITE_INT_FIELD(events);
  WRITE_NODE_FIELD(columns);
  WRITE_NODE_FIELD(whenClause);
  WRITE_BOOL_FIELD(isconstraint);
  WRITE_BOOL_FIELD(deferrable);
  WRITE_BOOL_FIELD(initdeferred);
  WRITE_NODE_FIELD(constrrel);
}

static void
_outCreatePLangStmt(StringInfo str, const CreatePLangStmt *node)
{
  WRITE_NODE_TYPE("CREATEPLANGSTMT");

  WRITE_BOOL_FIELD(replace);
  WRITE_STRING_FIELD(plname);
  WRITE_NODE_FIELD(plhandler);
  WRITE_NODE_FIELD(plinline);
  WRITE_NODE_FIELD(plvalidator);
  WRITE_BOOL_FIELD(pltrusted);
}

static void
_outCreateRoleStmt(StringInfo str, const CreateRoleStmt *node)
{
  WRITE_NODE_TYPE("CREATEROLESTMT");

  WRITE_ENUM_FIELD(stmt_type, RoleStmtType);
  WRITE_STRING_FIELD(role);
  WRITE_NODE_FIELD(options);
}

static void
_outAlterRoleStmt(StringInfo str, const AlterRoleStmt *node)
{
  WRITE_NODE_TYPE("ALTERROLESTMT");

  WRITE_STRING_FIELD(role);
  WRITE_NODE_FIELD(options);
  WRITE_INT_FIELD(action);
}

static void
_outDropRoleStmt(StringInfo str, const DropRoleStmt *node)
{
  WRITE_NODE_TYPE("DROPROLESTMT");

  WRITE_NODE_FIELD(roles);
  WRITE_BOOL_FIELD(missing_ok);
}

static void
_outLockStmt(StringInfo str, const LockStmt *node)
{
  WRITE_NODE_TYPE("LOCK");

  WRITE_NODE_FIELD(relations);
  WRITE_INT_FIELD(mode);
  WRITE_BOOL_FIELD(nowait);
}

static void
_outConstraintsSetStmt(StringInfo str, const ConstraintsSetStmt *node)
{
  WRITE_NODE_TYPE("CONSTRAINTSSETSTMT");

  WRITE_NODE_FIELD(constraints);
  WRITE_BOOL_FIELD(deferred);
}

static void
_outReindexStmt(StringInfo str, const ReindexStmt *node)
{
  WRITE_NODE_TYPE("REINDEXSTMT");

  WRITE_ENUM_FIELD(kind, ObjectType);
  WRITE_NODE_FIELD(relation);
  WRITE_STRING_FIELD(name);
  WRITE_BOOL_FIELD(do_system);
  WRITE_BOOL_FIELD(do_user);
}

static void
_outCheckPointStmt(StringInfo str, const CheckPointStmt *node)
{
  WRITE_NODE_TYPE("CHECKPOINT");

}

static void
_outCreateSchemaStmt(StringInfo str, const CreateSchemaStmt *node)
{
  WRITE_NODE_TYPE("CREATE SCHEMA");

  WRITE_STRING_FIELD(schemaname);
  WRITE_STRING_FIELD(authid);
  WRITE_NODE_FIELD(schemaElts);
  WRITE_BOOL_FIELD(if_not_exists);
}

static void
_outAlterDatabaseStmt(StringInfo str, const AlterDatabaseStmt *node)
{
  WRITE_NODE_TYPE("ALTERDATABASESTMT");

  WRITE_STRING_FIELD(dbname);
  WRITE_NODE_FIELD(options);
}

static void
_outAlterDatabaseSetStmt(StringInfo str, const AlterDatabaseSetStmt *node)
{
  WRITE_NODE_TYPE("ALTERDATABASESETSTMT");

  WRITE_STRING_FIELD(dbname);
  WRITE_NODE_FIELD(setstmt);
}

static void
_outAlterRoleSetStmt(StringInfo str, const AlterRoleSetStmt *node)
{
  WRITE_NODE_TYPE("ALTERROLESETSTMT");

  WRITE_STRING_FIELD(role);
  WRITE_STRING_FIELD(database);
  WRITE_NODE_FIELD(setstmt);
}

static void
_outCreateConversionStmt(StringInfo str, const CreateConversionStmt *node)
{
  WRITE_NODE_TYPE("CREATECONVERSIONSTMT");

  WRITE_NODE_FIELD(conversion_name);
  WRITE_STRING_FIELD(for_encoding_name);
  WRITE_STRING_FIELD(to_encoding_name);
  WRITE_NODE_FIELD(func_name);
  WRITE_BOOL_FIELD(def);
}

static void
_outCreateCastStmt(StringInfo str, const CreateCastStmt *node)
{
  WRITE_NODE_TYPE("CREATECASTSTMT");

  WRITE_NODE_FIELD(sourcetype);
  WRITE_NODE_FIELD(targettype);
  WRITE_NODE_FIELD(func);
  WRITE_ENUM_FIELD(context, CoercionContext);
  WRITE_BOOL_FIELD(inout);
}

static void
_outCreateOpClassStmt(StringInfo str, const CreateOpClassStmt *node)
{
  WRITE_NODE_TYPE("CREATEOPCLASSSTMT");

  WRITE_NODE_FIELD(opclassname);
  WRITE_NODE_FIELD(opfamilyname);
  WRITE_STRING_FIELD(amname);
  WRITE_NODE_FIELD(datatype);
  WRITE_NODE_FIELD(items);
  WRITE_BOOL_FIELD(isDefault);
}

static void
_outCreateOpFamilyStmt(StringInfo str, const CreateOpFamilyStmt *node)
{
  WRITE_NODE_TYPE("CREATEOPFAMILYSTMT");

  WRITE_NODE_FIELD(opfamilyname);
  WRITE_STRING_FIELD(amname);
}

static void
_outAlterOpFamilyStmt(StringInfo str, const AlterOpFamilyStmt *node)
{
  WRITE_NODE_TYPE("ALTEROPFAMILYSTMT");

  WRITE_NODE_FIELD(opfamilyname);
  WRITE_STRING_FIELD(amname);
  WRITE_BOOL_FIELD(isDrop);
  WRITE_NODE_FIELD(items);
}

static void
_outPrepareStmt(StringInfo str, const PrepareStmt *node)
{
  WRITE_NODE_TYPE("PREPARESTMT");

  WRITE_STRING_FIELD(name);
  WRITE_NODE_FIELD(argtypes);
  WRITE_NODE_FIELD(query);
}

static void
_outExecuteStmt(StringInfo str, const ExecuteStmt *node)
{
  WRITE_NODE_TYPE("EXECUTESTMT");

  WRITE_STRING_FIELD(name);
  WRITE_NODE_FIELD(params);
}

static void
_outDeallocateStmt(StringInfo str, const DeallocateStmt *node)
{
  WRITE_NODE_TYPE("DEALLOCATESTMT");

  WRITE_STRING_FIELD(name);
}

static void
_outDeclareCursorStmt(StringInfo str, const DeclareCursorStmt *node)
{
  WRITE_NODE_TYPE("DECLARECURSOR");

  WRITE_STRING_FIELD(portalname);
  WRITE_INT_FIELD(options);
  WRITE_NODE_FIELD(query);
}

static void
_outCreateTableSpaceStmt(StringInfo str, const CreateTableSpaceStmt *node)
{
  WRITE_NODE_TYPE("CREATETABLESPACESTMT");

  WRITE_STRING_FIELD(tablespacename);
  WRITE_STRING_FIELD(owner);
  WRITE_STRING_FIELD(location);
  WRITE_NODE_FIELD(options);
}

static void
_outDropTableSpaceStmt(StringInfo str, const DropTableSpaceStmt *node)
{
  WRITE_NODE_TYPE("DROPTABLESPACESTMT");

  WRITE_STRING_FIELD(tablespacename);
  WRITE_BOOL_FIELD(missing_ok);
}

static void
_outAlterObjectSchemaStmt(StringInfo str, const AlterObjectSchemaStmt *node)
{
  WRITE_NODE_TYPE("ALTEROBJECTSCHEMASTMT");

  WRITE_ENUM_FIELD(objectType, ObjectType);
  WRITE_NODE_FIELD(relation);
  WRITE_NODE_FIELD(object);
  WRITE_NODE_FIELD(objarg);
  WRITE_STRING_FIELD(newschema);
  WRITE_BOOL_FIELD(missing_ok);
}

static void
_outAlterOwnerStmt(StringInfo str, const AlterOwnerStmt *node)
{
  WRITE_NODE_TYPE("ALTEROWNERSTMT");

  WRITE_ENUM_FIELD(objectType, ObjectType);
  WRITE_NODE_FIELD(relation);
  WRITE_NODE_FIELD(object);
  WRITE_NODE_FIELD(objarg);
  WRITE_STRING_FIELD(newowner);
}

static void
_outDropOwnedStmt(StringInfo str, const DropOwnedStmt *node)
{
  WRITE_NODE_TYPE("DROPOWNEDSTMT");

  WRITE_NODE_FIELD(roles);
  WRITE_ENUM_FIELD(behavior, DropBehavior);
}

static void
_outReassignOwnedStmt(StringInfo str, const ReassignOwnedStmt *node)
{
  WRITE_NODE_TYPE("REASSIGNOWNEDSTMT");

  WRITE_NODE_FIELD(roles);
  WRITE_STRING_FIELD(newrole);
}

static void
_outCompositeTypeStmt(StringInfo str, const CompositeTypeStmt *node)
{
  WRITE_NODE_TYPE("COMPOSITETYPESTMT");

  WRITE_NODE_FIELD(typevar);
  WRITE_NODE_FIELD(coldeflist);
}

static void
_outCreateEnumStmt(StringInfo str, const CreateEnumStmt *node)
{
  WRITE_NODE_TYPE("CREATEENUMSTMT");

  WRITE_NODE_FIELD(typeName);
  WRITE_NODE_FIELD(vals);
}

static void
_outCreateRangeStmt(StringInfo str, const CreateRangeStmt *node)
{
  WRITE_NODE_TYPE("CREATERANGESTMT");

  WRITE_NODE_FIELD(typeName);
  WRITE_NODE_FIELD(params);
}

static void
_outAlterEnumStmt(StringInfo str, const AlterEnumStmt *node)
{
  WRITE_NODE_TYPE("ALTERENUMSTMT");

  WRITE_NODE_FIELD(typeName);
  WRITE_STRING_FIELD(newVal);
  WRITE_STRING_FIELD(newValNeighbor);
  WRITE_BOOL_FIELD(newValIsAfter);
  WRITE_BOOL_FIELD(skipIfExists);
}

static void
_outAlterTSDictionaryStmt(StringInfo str, const AlterTSDictionaryStmt *node)
{
  WRITE_NODE_TYPE("ALTERTSDICTIONARYSTMT");

  WRITE_NODE_FIELD(dictname);
  WRITE_NODE_FIELD(options);
}

static void
_outAlterTSConfigurationStmt(StringInfo str, const AlterTSConfigurationStmt *node)
{
  WRITE_NODE_TYPE("ALTERTSCONFIGURATIONSTMT");

  WRITE_NODE_FIELD(cfgname);
  WRITE_NODE_FIELD(tokentype);
  WRITE_NODE_FIELD(dicts);
  WRITE_BOOL_FIELD(override);
  WRITE_BOOL_FIELD(replace);
  WRITE_BOOL_FIELD(missing_ok);
}

static void
_outCreateFdwStmt(StringInfo str, const CreateFdwStmt *node)
{
  WRITE_NODE_TYPE("CREATEFDWSTMT");

  WRITE_STRING_FIELD(fdwname);
  WRITE_NODE_FIELD(func_options);
  WRITE_NODE_FIELD(options);
}

static void
_outAlterFdwStmt(StringInfo str, const AlterFdwStmt *node)
{
  WRITE_NODE_TYPE("ALTERFDWSTMT");

  WRITE_STRING_FIELD(fdwname);
  WRITE_NODE_FIELD(func_options);
  WRITE_NODE_FIELD(options);
}

static void
_outCreateForeignServerStmt(StringInfo str, const CreateForeignServerStmt *node)
{
  WRITE_NODE_TYPE("CREATEFOREIGNSERVERSTMT");

  WRITE_STRING_FIELD(servername);
  WRITE_STRING_FIELD(servertype);
  WRITE_STRING_FIELD(version);
  WRITE_STRING_FIELD(fdwname);
  WRITE_NODE_FIELD(options);
}

static void
_outAlterForeignServerStmt(StringInfo str, const AlterForeignServerStmt *node)
{
  WRITE_NODE_TYPE("ALTERFOREIGNSERVERSTMT");

  WRITE_STRING_FIELD(servername);
  WRITE_STRING_FIELD(version);
  WRITE_NODE_FIELD(options);
  WRITE_BOOL_FIELD(has_version);
}

static void
_outCreateUserMappingStmt(StringInfo str, const CreateUserMappingStmt *node)
{
  WRITE_NODE_TYPE("CREATEUSERMAPPINGSTMT");

  WRITE_STRING_FIELD(username);
  WRITE_STRING_FIELD(servername);
  WRITE_NODE_FIELD(options);
}

static void
_outAlterUserMappingStmt(StringInfo str, const AlterUserMappingStmt *node)
{
  WRITE_NODE_TYPE("ALTERUSERMAPPINGSTMT");

  WRITE_STRING_FIELD(username);
  WRITE_STRING_FIELD(servername);
  WRITE_NODE_FIELD(options);
}

static void
_outDropUserMappingStmt(StringInfo str, const DropUserMappingStmt *node)
{
  WRITE_NODE_TYPE("DROPUSERMAPPINGSTMT");

  WRITE_STRING_FIELD(username);
  WRITE_STRING_FIELD(servername);
  WRITE_BOOL_FIELD(missing_ok);
}

static void
_outAlterTableSpaceOptionsStmt(StringInfo str, const AlterTableSpaceOptionsStmt *node)
{
  WRITE_NODE_TYPE("ALTERTABLESPACEOPTIONSSTMT");

  WRITE_STRING_FIELD(tablespacename);
  WRITE_NODE_FIELD(options);
  WRITE_BOOL_FIELD(isReset);
}

static void
_outAlterTableMoveAllStmt(StringInfo str, const AlterTableMoveAllStmt *node)
{
  WRITE_NODE_TYPE("ALTERTABLEMOVEALLSTMT");

  WRITE_STRING_FIELD(orig_tablespacename);
  WRITE_ENUM_FIELD(objtype, ObjectType);
  WRITE_NODE_FIELD(roles);
  WRITE_STRING_FIELD(new_tablespacename);
  WRITE_BOOL_FIELD(nowait);
}

static void
_outSecLabelStmt(StringInfo str, const SecLabelStmt *node)
{
  WRITE_NODE_TYPE("SECLABELSTMT");

  WRITE_ENUM_FIELD(objtype, ObjectType);
  WRITE_NODE_FIELD(objname);
  WRITE_NODE_FIELD(objargs);
  WRITE_STRING_FIELD(provider);
  WRITE_STRING_FIELD(label);
}

static void
_outCreateForeignTableStmt(StringInfo str, const CreateForeignTableStmt *node)
{
  WRITE_NODE_TYPE("CREATEFOREIGNTABLESTMT");

  _outCreateStmtInfo(str, (const CreateStmt *) node);

  WRITE_STRING_FIELD(servername);
  WRITE_NODE_FIELD(options);
}

static void
_outCreateExtensionStmt(StringInfo str, const CreateExtensionStmt *node)
{
  WRITE_NODE_TYPE("CREATEEXTENSIONSTMT");

  WRITE_STRING_FIELD(extname);
  WRITE_BOOL_FIELD(if_not_exists);
  WRITE_NODE_FIELD(options);
}

static void
_outAlterExtensionStmt(StringInfo str, const AlterExtensionStmt *node)
{
  WRITE_NODE_TYPE("ALTEREXTENSIONSTMT");

  WRITE_STRING_FIELD(extname);
  WRITE_NODE_FIELD(options);
}

static void
_outAlterExtensionContentsStmt(StringInfo str, const AlterExtensionContentsStmt *node)
{
  WRITE_NODE_TYPE("ALTEREXTENSIONCONTENTSSTMT");

  WRITE_STRING_FIELD(extname);
  WRITE_INT_FIELD(action);
  WRITE_ENUM_FIELD(objtype, ObjectType);
  WRITE_NODE_FIELD(objname);
  WRITE_NODE_FIELD(objargs);
}

static void
_outCreateEventTrigStmt(StringInfo str, const CreateEventTrigStmt *node)
{
  WRITE_NODE_TYPE("CREATEEVENTTRIGSTMT");

  WRITE_STRING_FIELD(trigname);
  WRITE_STRING_FIELD(eventname);
  WRITE_NODE_FIELD(whenclause);
  WRITE_NODE_FIELD(funcname);
}

static void
_outAlterEventTrigStmt(StringInfo str, const AlterEventTrigStmt *node)
{
  WRITE_NODE_TYPE("ALTEREVENTTRIGSTMT");

  WRITE_STRING_FIELD(trigname);
  WRITE_CHAR_FIELD(tgenabled);
}

static void
_outRefreshMatViewStmt(StringInfo str, const RefreshMatViewStmt *node)
{
  WRITE_NODE_TYPE("REFRESHMATVIEWSTMT");

  WRITE_BOOL_FIELD(concurrent);
  WRITE_BOOL_FIELD(skipData);
  WRITE_NODE_FIELD(relation);
}

static void
_outReplicaIdentityStmt(StringInfo str, const ReplicaIdentityStmt *node)
{
  WRITE_NODE_TYPE("REPLICAIDENTITYSTMT");

  WRITE_CHAR_FIELD(identity_type);
  WRITE_STRING_FIELD(name);
}

static void
_outAlterSystemStmt(StringInfo str, const AlterSystemStmt *node)
{
  WRITE_NODE_TYPE("ALTERSYSTEMSTMT");

  WRITE_NODE_FIELD(setstmt);
}

static void
_outColumnRef(StringInfo str, const ColumnRef *node)
{
  WRITE_NODE_TYPE("COLUMNREF");

  WRITE_NODE_FIELD(fields);
  WRITE_LOCATION_FIELD(location);
}

static void
_outParamRef(StringInfo str, const ParamRef *node)
{
  WRITE_NODE_TYPE("PARAMREF");

  WRITE_INT_FIELD(number);
  WRITE_LOCATION_FIELD(location);
}

static void
_outFuncCall(StringInfo str, const FuncCall *node)
{
  WRITE_NODE_TYPE("FUNCCALL");

  WRITE_NODE_FIELD(funcname);
  WRITE_NODE_FIELD(args);
  WRITE_NODE_FIELD(agg_order);
  WRITE_NODE_FIELD(agg_filter);
  WRITE_BOOL_FIELD(agg_within_group);
  WRITE_BOOL_FIELD(agg_star);
  WRITE_BOOL_FIELD(agg_distinct);
  WRITE_BOOL_FIELD(func_variadic);
  WRITE_NODE_FIELD(over);
  WRITE_LOCATION_FIELD(location);
}

static void
_outA_Star(StringInfo str, const A_Star *node)
{
  WRITE_NODE_TYPE("A_STAR");

}

static void
_outA_Indices(StringInfo str, const A_Indices *node)
{
  WRITE_NODE_TYPE("A_INDICES");

  WRITE_NODE_FIELD(lidx);
  WRITE_NODE_FIELD(uidx);
}

static void
_outA_Indirection(StringInfo str, const A_Indirection *node)
{
  WRITE_NODE_TYPE("A_INDIRECTION");

  WRITE_NODE_FIELD(arg);
  WRITE_NODE_FIELD(indirection);
}

static void
_outA_ArrayExpr(StringInfo str, const A_ArrayExpr *node)
{
  WRITE_NODE_TYPE("A_ARRAYEXPR");

  WRITE_NODE_FIELD(elements);
  WRITE_LOCATION_FIELD(location);
}

static void
_outResTarget(StringInfo str, const ResTarget *node)
{
  WRITE_NODE_TYPE("RESTARGET");

  WRITE_STRING_FIELD(name);
  WRITE_NODE_FIELD(indirection);
  WRITE_NODE_FIELD(val);
  WRITE_LOCATION_FIELD(location);
}

static void
_outTypeCast(StringInfo str, const TypeCast *node)
{
  WRITE_NODE_TYPE("TYPECAST");

  WRITE_NODE_FIELD(arg);
  WRITE_NODE_FIELD(typeName);
  WRITE_LOCATION_FIELD(location);
}

static void
_outCollateClause(StringInfo str, const CollateClause *node)
{
  WRITE_NODE_TYPE("COLLATECLAUSE");

  WRITE_NODE_FIELD(arg);
  WRITE_NODE_FIELD(collname);
  WRITE_LOCATION_FIELD(location);
}

static void
_outSortBy(StringInfo str, const SortBy *node)
{
  WRITE_NODE_TYPE("SORTBY");

  WRITE_NODE_FIELD(node);
  WRITE_ENUM_FIELD(sortby_dir, SortByDir);
  WRITE_ENUM_FIELD(sortby_nulls, SortByNulls);
  WRITE_NODE_FIELD(useOp);
  WRITE_LOCATION_FIELD(location);
}

static void
_outWindowDef(StringInfo str, const WindowDef *node)
{
  WRITE_NODE_TYPE("WINDOWDEF");

  WRITE_STRING_FIELD(name);
  WRITE_STRING_FIELD(refname);
  WRITE_NODE_FIELD(partitionClause);
  WRITE_NODE_FIELD(orderClause);
  WRITE_INT_FIELD(frameOptions);
  WRITE_NODE_FIELD(startOffset);
  WRITE_NODE_FIELD(endOffset);
  WRITE_LOCATION_FIELD(location);
}

static void
_outRangeSubselect(StringInfo str, const RangeSubselect *node)
{
  WRITE_NODE_TYPE("RANGESUBSELECT");

  WRITE_BOOL_FIELD(lateral);
  WRITE_NODE_FIELD(subquery);
  WRITE_NODE_FIELD(alias);
}

static void
_outRangeFunction(StringInfo str, const RangeFunction *node)
{
  WRITE_NODE_TYPE("RANGEFUNCTION");

  WRITE_BOOL_FIELD(lateral);
  WRITE_BOOL_FIELD(ordinality);
  WRITE_BOOL_FIELD(is_rowsfrom);
  WRITE_NODE_FIELD(functions);
  WRITE_NODE_FIELD(alias);
  WRITE_NODE_FIELD(coldeflist);
}

static void
_outTypeName(StringInfo str, const TypeName *node)
{
  WRITE_NODE_TYPE("TYPENAME");

  WRITE_NODE_FIELD(names);
  WRITE_OID_FIELD(typeOid);
  WRITE_BOOL_FIELD(setof);
  WRITE_BOOL_FIELD(pct_type);
  WRITE_NODE_FIELD(typmods);
  WRITE_INT_FIELD(typemod);
  WRITE_NODE_FIELD(arrayBounds);
  WRITE_LOCATION_FIELD(location);
}

static void
_outColumnDef(StringInfo str, const ColumnDef *node)
{
  WRITE_NODE_TYPE("COLUMNDEF");

  WRITE_STRING_FIELD(colname);
  WRITE_NODE_FIELD(typeName);
  WRITE_INT_FIELD(inhcount);
  WRITE_BOOL_FIELD(is_local);
  WRITE_BOOL_FIELD(is_not_null);
  WRITE_BOOL_FIELD(is_from_type);
  WRITE_CHAR_FIELD(storage);
  WRITE_NODE_FIELD(raw_default);
  WRITE_NODE_FIELD(cooked_default);
  WRITE_NODE_FIELD(collClause);
  WRITE_OID_FIELD(collOid);
  WRITE_NODE_FIELD(constraints);
  WRITE_NODE_FIELD(fdwoptions);
  WRITE_LOCATION_FIELD(location);
}

static void
_outIndexElem(StringInfo str, const IndexElem *node)
{
  WRITE_NODE_TYPE("INDEXELEM");

  WRITE_STRING_FIELD(name);
  WRITE_NODE_FIELD(expr);
  WRITE_STRING_FIELD(indexcolname);
  WRITE_NODE_FIELD(collation);
  WRITE_NODE_FIELD(opclass);
  WRITE_ENUM_FIELD(ordering, SortByDir);
  WRITE_ENUM_FIELD(nulls_ordering, SortByNulls);
}

static void
_outDefElem(StringInfo str, const DefElem *node)
{
  WRITE_NODE_TYPE("DEFELEM");

  WRITE_STRING_FIELD(defnamespace);
  WRITE_STRING_FIELD(defname);
  WRITE_NODE_FIELD(arg);
  WRITE_ENUM_FIELD(defaction, DefElemAction);
}

static void
_outRangeTblFunction(StringInfo str, const RangeTblFunction *node)
{
  WRITE_NODE_TYPE("RANGETBLFUNCTION");

  WRITE_NODE_FIELD(funcexpr);
  WRITE_INT_FIELD(funccolcount);
  WRITE_NODE_FIELD(funccolnames);
  WRITE_NODE_FIELD(funccoltypes);
  WRITE_NODE_FIELD(funccoltypmods);
  WRITE_NODE_FIELD(funccolcollations);
  WRITE_BITMAPSET_FIELD(funcparams);
}

static void
_outWithCheckOption(StringInfo str, const WithCheckOption *node)
{
  WRITE_NODE_TYPE("WITHCHECKOPTION");

  WRITE_STRING_FIELD(viewname);
  WRITE_NODE_FIELD(qual);
  WRITE_BOOL_FIELD(cascaded);
}

static void
_outSortGroupClause(StringInfo str, const SortGroupClause *node)
{
  WRITE_NODE_TYPE("SORTGROUPCLAUSE");

  WRITE_UINT_FIELD(tleSortGroupRef);
  WRITE_OID_FIELD(eqop);
  WRITE_OID_FIELD(sortop);
  WRITE_BOOL_FIELD(nulls_first);
  WRITE_BOOL_FIELD(hashable);
}

static void
_outWindowClause(StringInfo str, const WindowClause *node)
{
  WRITE_NODE_TYPE("WINDOWCLAUSE");

  WRITE_STRING_FIELD(name);
  WRITE_STRING_FIELD(refname);
  WRITE_NODE_FIELD(partitionClause);
  WRITE_NODE_FIELD(orderClause);
  WRITE_INT_FIELD(frameOptions);
  WRITE_NODE_FIELD(startOffset);
  WRITE_NODE_FIELD(endOffset);
  WRITE_UINT_FIELD(winref);
  WRITE_BOOL_FIELD(copiedOrder);
}

static void
_outPrivGrantee(StringInfo str, const PrivGrantee *node)
{
  WRITE_NODE_TYPE("PRIVGRANTEE");

  WRITE_STRING_FIELD(rolname);
}

static void
_outFuncWithArgs(StringInfo str, const FuncWithArgs *node)
{
  WRITE_NODE_TYPE("FUNCWITHARGS");

  WRITE_NODE_FIELD(funcname);
  WRITE_NODE_FIELD(funcargs);
}

static void
_outAccessPriv(StringInfo str, const AccessPriv *node)
{
  WRITE_NODE_TYPE("ACCESSPRIV");

  WRITE_STRING_FIELD(priv_name);
  WRITE_NODE_FIELD(cols);
}

static void
_outCreateOpClassItem(StringInfo str, const CreateOpClassItem *node)
{
  WRITE_NODE_TYPE("CREATEOPCLASSITEM");

  WRITE_INT_FIELD(itemtype);
  WRITE_NODE_FIELD(name);
  WRITE_NODE_FIELD(args);
  WRITE_INT_FIELD(number);
  WRITE_NODE_FIELD(order_family);
  WRITE_NODE_FIELD(class_args);
  WRITE_NODE_FIELD(storedtype);
}

static void
_outTableLikeClause(StringInfo str, const TableLikeClause *node)
{
  WRITE_NODE_TYPE("TABLELIKECLAUSE");

  WRITE_NODE_FIELD(relation);
  WRITE_UINT_FIELD(options);
}

static void
_outFunctionParameter(StringInfo str, const FunctionParameter *node)
{
  WRITE_NODE_TYPE("FUNCTIONPARAMETER");

  WRITE_STRING_FIELD(name);
  WRITE_NODE_FIELD(argType);
  WRITE_ENUM_FIELD(mode, FunctionParameterMode);
  WRITE_NODE_FIELD(defexpr);
}

static void
_outLockingClause(StringInfo str, const LockingClause *node)
{
  WRITE_NODE_TYPE("LOCKINGCLAUSE");

  WRITE_NODE_FIELD(lockedRels);
  WRITE_ENUM_FIELD(strength, LockClauseStrength);
  WRITE_BOOL_FIELD(noWait);
}

static void
_outRowMarkClause(StringInfo str, const RowMarkClause *node)
{
  WRITE_NODE_TYPE("ROWMARKCLAUSE");

  WRITE_UINT_FIELD(rti);
  WRITE_ENUM_FIELD(strength, LockClauseStrength);
  WRITE_BOOL_FIELD(noWait);
  WRITE_BOOL_FIELD(pushedDown);
}

static void
_outXmlSerialize(StringInfo str, const XmlSerialize *node)
{
  WRITE_NODE_TYPE("XMLSERIALIZE");

  WRITE_ENUM_FIELD(xmloption, XmlOptionType);
  WRITE_NODE_FIELD(expr);
  WRITE_NODE_FIELD(typeName);
  WRITE_LOCATION_FIELD(location);
}

static void
_outWithClause(StringInfo str, const WithClause *node)
{
  WRITE_NODE_TYPE("WITHCLAUSE");

  WRITE_NODE_FIELD(ctes);
  WRITE_BOOL_FIELD(recursive);
  WRITE_LOCATION_FIELD(location);
}

static void
_outCommonTableExpr(StringInfo str, const CommonTableExpr *node)
{
  WRITE_NODE_TYPE("COMMONTABLEEXPR");

  WRITE_STRING_FIELD(ctename);
  WRITE_NODE_FIELD(aliascolnames);
  WRITE_NODE_FIELD(ctequery);
  WRITE_LOCATION_FIELD(location);
  WRITE_BOOL_FIELD(cterecursive);
  WRITE_INT_FIELD(cterefcount);
  WRITE_NODE_FIELD(ctecolnames);
  WRITE_NODE_FIELD(ctecoltypes);
  WRITE_NODE_FIELD(ctecoltypmods);
  WRITE_NODE_FIELD(ctecolcollations);
}

static void
_outInlineCodeBlock(StringInfo str, const InlineCodeBlock *node)
{
  WRITE_NODE_TYPE("INLINECODEBLOCK");

  WRITE_STRING_FIELD(source_text);
  WRITE_OID_FIELD(langOid);
  WRITE_BOOL_FIELD(langIsTrusted);
}

