(* ::Package:: *)

(* ::Title:: *)
(*Advanced Mapping*)


(* ::Chapter:: *)
(*Begin package*)


BeginPackage["AdvancedMapping`"]


(* ::Chapter:: *)
(*Package description*)


ProgressMap::usage =
 "ProgressMap[f,expr] is a Map implementation with progress bar. levelspec is always {1}.
\"ShowInfo\"\[Rule]True show the detailed version of the progress. \"Label\"->\"Custom label\" shows a custom description label.";

ProgressParallelMap::usage =
 "ProgressParallelMap[f,expr] is a ParallelMap implementation with progress bar. levelspec is always {1}. 
\"ShowInfo\"\[Rule]True show the detailed version of the progress bar. \"Label\"->\"Custom label\" shows a custom description label. It inherits all options of ParallelMap.";

ParallelMapThread::usage =
"ParallelMapThread[f, {{\!\(\*SubscriptBox[
StyleBox[\"a\", \"TI\"], 
StyleBox[\"1\", \"TR\"]]\),\!\(\*SubscriptBox[
StyleBox[\"a\", \"TI\"], 
StyleBox[\"2\", \"TR\"]]\),\!\(\*
StyleBox[\"\[Ellipsis]\", \"TR\"]\)},{\!\(\*SubscriptBox[
StyleBox[\"b\", \"TI\"], 
StyleBox[\"1\", \"TR\"]]\),\!\(\*SubscriptBox[
StyleBox[\"b\", \"TI\"], 
StyleBox[\"2\", \"TR\"]]\),\!\(\*
StyleBox[\"\[Ellipsis]\", \"TR\"]\)},\!\(\*
StyleBox[\"\[Ellipsis]\", \"TR\"]\)}] is a parallel implementation of MapThread. Options are the same as ParallelMap.";

ProgressMapThread::usage = 
"ProgressMapThread[f, {{\!\(\*SubscriptBox[
StyleBox[\"a\", \"TI\"], 
StyleBox[\"1\", \"TR\"]]\),\!\(\*SubscriptBox[
StyleBox[\"a\", \"TI\"], 
StyleBox[\"2\", \"TR\"]]\),\!\(\*
StyleBox[\"\[Ellipsis]\", \"TR\"]\)},{\!\(\*SubscriptBox[
StyleBox[\"b\", \"TI\"], 
StyleBox[\"1\", \"TR\"]]\),\!\(\*SubscriptBox[
StyleBox[\"b\", \"TI\"], 
StyleBox[\"2\", \"TR\"]]\),\!\(\*
StyleBox[\"\[Ellipsis]\", \"TR\"]\)},\!\(\*
StyleBox[\"\[Ellipsis]\", \"TR\"]\)}] is a MapThread implementation with progress bar. levelspec is always {1}. 
\"ShowInfo\"\[Rule]True show the detailed version of the progress bar. \"Label\"->\"Custom label\" shows a custom description label."

ProgressParallelMapThread::usage = 
"ProgressParallelMapThread[f, {{\!\(\*SubscriptBox[
StyleBox[\"a\", \"TI\"], 
StyleBox[\"1\", \"TR\"]]\),\!\(\*SubscriptBox[
StyleBox[\"a\", \"TI\"], 
StyleBox[\"2\", \"TR\"]]\),\!\(\*
StyleBox[\"\[Ellipsis]\", \"TR\"]\)},{\!\(\*SubscriptBox[
StyleBox[\"b\", \"TI\"], 
StyleBox[\"1\", \"TR\"]]\),\!\(\*SubscriptBox[
StyleBox[\"b\", \"TI\"], 
StyleBox[\"2\", \"TR\"]]\),\!\(\*
StyleBox[\"\[Ellipsis]\", \"TR\"]\)},\!\(\*
StyleBox[\"\[Ellipsis]\", \"TR\"]\)}] is a ParallelMapThread implementation with progress bar. levelspec is always {1}. 
\"ShowInfo\"\[Rule]True show the detailed version of the progress bar. \"Label\"->\"Custom label\" shows a custom description label."
  
ProgressTable::usage =
 "Table implementation with progress bar. Same usage as Table. 
\"ShowInfo\"\[Rule]True show the detailed version of the progress bar. \"Label\"->\"Custom label\" shows a custom description label.";

ProgressParallelTable::usage =
"ParallelTable implementation with progress bar. Same usage as ParallelTable. 
\"ShowInfo\"\[Rule]True show the detailed version of the progress bar. \"Label\"->\"Custom label\" shows a custom description label.
It inherits all the options from ParallelTable.";

MapIf::usage = "MapIf[f, expr, crit] Maps if condition in crit is true at each element.";

MapIfElse::usage = "MapIfElse[f,g,expr,crit] Maps f if condition in crit is true at each element.
Else it maps g.";

MapPattern::usage = "MapPattern[f, expr, patt] Maps when pattern in patt is matched.";

NestApplyList::usage = "NestApplyList[f, g, expr, n] is equivalent to Map[g, NestList[f, expr, n]], 
but doesn't keep the whole NestList[f, expr, n] list, using less memory.";

NestApplyWhileList::usage = "NestApplyWhileList[f, g, expr, test] is equivalent to 
Map[g, NestWhileList[f, expr, test]], but doesn't keep the whole NestList[f, expr, n] list, using less memory.";

NestListIndexed::usage = "NestListIndexed[f, expr, n, startIndex] works as NestList but returns the number of the index
alongside the result of NestList.";

FoldWhile::usage = "FoldWhile[f, test, start, secargs, max] is a equivalent to Fold that folds while test is True.";

FoldWhileList::usage = "FoldWhileList[f, test, start, secargs, max] is a equivalent to FoldList that folds while test is True.";

EvaluateOnce::usage = "EvaluateOnce[expr] evaluates expr just once. Further reevaluation is not possible.";

EvaluateIfChanged::usage = "EvaluateIfChanged[expr, trackedsymbols] evaluates expr only if there is a change in trackedsymbols.";

LinkParallelEvaluate::usage = "LinkParallelEvaluate[expr, \"name\"] loads a WSTP-compatible external program in every subkernel to perform a parallel
evaluation on expr. expr must be a parallel function like ParallelMap, ParallelTable, ParallelEvaluate, etc.";

LinkEvaluate::usage "LinkEvaluate[expr, \"name\"] loads a WSTP-compatible external program in the master kernel to perform a
evaluation on expr. This can be used with LinkParallelEvaluate to control the scoping of the link definitions.";


(* ::Chapter:: *)
(*Error messages*)


ProgressParallelMap::exprlengtherr = "expr must have at least a length greater than 1";
ProgressMap::exprlengtherr = "expr must have at least a length greater than 1";
ProgressMapThread::exprlengtherr = "expr must have at least a length greater than 1";
ProgressMapThread::exprdeptherr = "expr must have a depth of at least 3, current depth is `1`";
ProgressParallelMapThread::exprlengtherr = "expr must have at least a length greater than 1";
ProgressParallelMapThread::exprdeptherr = "expr must have a depth of at least 3, current depth is `1`";


(* ::Chapter:: *)
(*Begin package*)


Begin["`Private`"]


(* ::Chapter:: *)
(*Clock*)


(* ::Text:: *)
(*Simple hh/mm/ss implementation of a clock.*)


GetSeconds[time_] := IntegerString[Round[Mod[time, 60]], 10, 2];
GetMinutes[time_]:= IntegerString[Mod[Floor[time/60], 60], 10, 2];
GetHours[time_] := IntegerString[Floor[time/3600], 10, 2];
ClockFormat[time_] := StringJoin[GetHours[time], ":", GetMinutes[time], ":", GetSeconds[time]];


(* ::Chapter:: *)
(*Progress indicator*)


(* ::Text:: *)
(*Progress indicator to be used by ProgressMap and ProgressTable family of functions.*)


DefaultIndicator[indexProgress_, totalSize_] := ProgressIndicator[indexProgress, {1, totalSize}];

DetailedIndicator[indexProgress_, totalSize_, startTime_, label_:"Evaluating..."] := 
Block[{progressString, remainingTime, remainingTimeString, indicator, ellapsedTimeString},
	progressString = Row[{Style["Progress: ", Bold], ToString[indexProgress], "/", ToString[totalSize]}];
	ellapsedTimeString = Row[{Style["Elapsed time: ", Bold], ClockFormat[AbsoluteTime[] - startTime]}];

	If[indexProgress != 0,
		remainingTime = ((AbsoluteTime[] - startTime) / indexProgress)*(totalSize - indexProgress);
		remainingTimeString = Row[{Style["Remaining: ", Bold], ClockFormat[remainingTime]}];
		,
		remainingTimeString = Row[{Style["Remaining: ", Bold], "Unknown"}];
	];

	indicator = Panel[
		Column[
			{
				Style[label, Bold],
				DefaultIndicator[indexProgress, totalSize],
				progressString,
				ellapsedTimeString,
				remainingTimeString
			}
		]
	];

	Return[indicator];
];


(* ::Chapter:: *)
(*Conditional and special maps*)


(* ::Text:: *)
(*Special cases of regular Map functions.*)


SetAttributes[MapIf, HoldAll];
MapIf[f_, expr_, crit_] := MapAt[f, expr, Position[Map[crit, expr], True]];

SetAttributes[MapIfElse, HoldAll];
MapIfElse[f1_, f2_, expr_, crit_] := Block[{truePos, falsePos},
	truePos = Position[Map[crit, expr], True];
	falsePos = Complement[Transpose[{Range[Length[expr]]}], truePos];
	MapAt[f2, MapAt[f1, expr, truePos], falsePos]
];

SetAttributes[MapPattern, HoldAll];
MapPattern[f_, expr_, patt_] := MapAt[f, expr, Position[expr, patt]];

SetAttributes[ParallelMapThread, HoldFirst];
ParallelMapThread::exprdeptherr = "expr must have a depth of at least 3, current depth is `1`";
ParallelMapThread[f_, expr_] := If[Depth[expr] < 3,
	Message[ParallelMapThread::exprdeptherr, Depth[expr]];Return[$Failed]
	,
	ParallelMap[Apply[f, #]&, Transpose[expr]]
];


(* ::Chapter:: *)
(*Conditional folding*)


(* ::Text:: *)
(*Source: https://mathematica.stackexchange.com/a/19105*)
(*Author: Leonid Shifr*)


dressInCtr[test_,max_] := Module[{ctr = 0}, (++ctr<=max) && test[##]&];

SetAttributes[FoldWhile, HoldFirst];
FoldWhile[f_, test_, start_, secargs_List, max_Integer] := FoldWhile[f, dressInCtr[test, max], start, secargs];

FoldWhile[f_, test_, start_, secargs_List] := Block[{last = start},
	Fold[
		If[
			test[##],
			last = f[##],
			Return[last,Fold]
			]&,
			start,
			secargs
		]
];

SetAttributes[FoldWhileList, HoldFirst];
FoldWhileList[f_, test_, start_, secargs_List, max_Integer] := FoldWhileList[f, dressInCtr[test, max], start, secargs];
FoldWhileList[f_, test_, start_, secargs_List] :=
Module[{result, tag},
	result = Reap[
		Fold[
			If[test[##],
				Sow[f[##],tag],
				Return[Null,Fold]
			]&,
			start,
			secargs
		],
		_,
		#2&
	][[2]];

	If[result==={},
		{start},
		Prepend[First[result], start]
	]
]


(* ::Chapter:: *)
(*Maps with progress*)


SetAttributes[ProgressParallelMap, HoldFirst];
ProgressParallelMap[f_, expr_, opts: OptionsPattern[{"ShowInfo"->True, "Label"->"Evaluating...", Parallelize}]] :=
Module[{startTime = AbsoluteTime[], indexProgress = 0, output},
	SetSharedVariable[indexProgress];
	If[Length[expr]<1, Message[ProgressParallelMap::exprlengtherr];Return[$Failed]];

	Monitor[
		ParallelMap[
			(
				output = f[#];
				indexProgress++;
				output
			)&,
			expr,
			FilterRules[{opts}, Options[Parallelize]]
		]
		,
		If[OptionValue["ShowInfo"],
			DetailedIndicator[indexProgress, Length[expr], startTime, OptionValue["Label"]]
			,
			DefaultIndicator[indexProgress, Length[expr]]
		]
	]
]; 

SetAttributes[ProgressMap, HoldFirst];
ProgressMap[f_, expr_, OptionsPattern[{"ShowInfo"->True, "Label"->"Evaluating..."}]] :=
Module[{startTime = AbsoluteTime[], indexProgress = 0, output},
	Monitor[
		If[Length[expr]<1, Message[ProgressMap::exprlengtherr];Return[$Failed]];
		Map[
			(
				output = f[#];
				indexProgress++;
				output
			)&,
			expr
		]
		,
		If[OptionValue["ShowInfo"],
			DetailedIndicator[indexProgress, Length[expr], startTime, OptionValue["Label"]]
			,
			DefaultIndicator[indexProgress, Length[expr]]
		]
	]
];


(* ::Chapter:: *)
(*MapThread with progress*)


SetAttributes[ProgressMapThread,HoldFirst];
ProgressMapThread[f_, expr_, OptionsPattern[{"ShowInfo"->True,"Label"->"Evaluating..."}]] :=
Module[{startTime = AbsoluteTime[], indexProgress = 0, output},
	If[Length[expr] < 1, Message[ProgressMapThread::exprlengtherr];Return[$Failed]];
	If[Depth[expr] < 3, Message[ProgressMapThread::exprdeptherr, Depth[expr]];Return[$Failed]];

	Monitor[
		MapThread[
			(
				output = f[##];
				indexProgress++;
				output
			)&
			,
			expr
		]
		,
		If[OptionValue["ShowInfo"],
			DetailedIndicator[indexProgress, Length[First[expr]], startTime, OptionValue["Label"]]
			,
			DefaultIndicator[indexProgress, Length[First[expr]]]
		]
	]
]; 

SetAttributes[ProgressParallelMapThread,HoldFirst];
ProgressParallelMapThread[f_, expr_, OptionsPattern[{"ShowInfo"->True,"Label"->"Evaluating..."}]] :=
Module[{startTime = AbsoluteTime[], indexProgress = 0, output},
	SetSharedVariable[indexProgress];
	If[Length[expr] < 1, Message[ProgressParallelMapThread::exprlengtherr];Return[$Failed]];
	If[Depth[expr] < 3, Message[ProgressParallelMapThread::exprdeptherr, Depth[expr]];Return[$Failed]];

	Monitor[
		ParallelMapThread[
		(
			output = f[##];
			indexProgress++;
			output
		)&
		,
		expr
		]
		,
		If[OptionValue["ShowInfo"],
			DetailedIndicator[indexProgress, Length[First[expr]], startTime, OptionValue["Label"]]
			,
			DefaultIndicator[indexProgress, Length[First[expr]]]
		]
	]
]; 


(* ::Chapter:: *)
(*Tables with progress*)


SetAttributes[ProgressTable, HoldFirst];
Options[ProgressTable] = {"ShowInfo"->True, "Label"->"Evaluating..."};

ProgressTable[expr_, n_Integer, opts: OptionsPattern[]] :=
Module[{tableIndex = 0, startTime = AbsoluteTime[]},
	Monitor[
		Table[tableIndex++;expr, n]
		,
		If[OptionValue[ProgressTable, {opts}, "ShowInfo"],
			DetailedIndicator[tableIndex, n, startTime, OptionValue[ProgressTable, {opts}, "Label"]]
			,
			DefaultIndicator[tableIndex, n]
		]
	]
];

ProgressTable[expr_, iterators__, opts:OptionsPattern[]]:= 
Module[{tuplesLength, tableIndex = 0, startTime = AbsoluteTime[]},
	(* This is beautiful. Author: Giovanni F. From: https://mathematica.stackexchange.com/a/86059 *)
	tuplesLength = Length[Tuples[Map[Range[Rest[#] /. List->Sequence]&, {iterators}]]];

	Monitor[
		Table[tableIndex++;expr, iterators]
		,
		If[OptionValue[ProgressTable, {opts}, "ShowInfo"],
			DetailedIndicator[tableIndex, tuplesLength, startTime, OptionValue[ProgressTable, {opts}, "Label"]]
			,
			DefaultIndicator[tableIndex, tuplesLength]
		]
	]
];


(* ::Chapter:: *)
(*Parallel tables with progress*)


SetAttributes[ProgressParallelTable,HoldFirst];

Options[ProgressTable]={"ShowInfo"->True, "Label"->"Evaluating...", ParallelTable};

ProgressParallelTable[expr_, n_Integer, opts:OptionsPattern[]]:=
Module[{tableIndex = 0,startTime = AbsoluteTime[]},
	SetSharedVariable[tableIndex];

	Monitor[
		ParallelTable[tableIndex++;expr, n, Evaluate[FilterRules[{opts}, Options[ParallelTable]]]]
		,
		If[OptionValue[ProgressTable, {opts}, "ShowInfo"],
			DetailedIndicator[tableIndex, n, startTime, OptionValue[ProgressTable, {opts}, "Label"]]
			,
			DefaultIndicator[tableIndex, n]
		]
	]
];

ProgressParallelTable[expr_, iterators__, opts:OptionsPattern[]]:= 
Module[{tuplesLength, tableIndex = 0, startTime = AbsoluteTime[]},
	SetSharedVariable[tableIndex];
	tuplesLength = Length[Tuples[Map[Range[Rest[#] /. List->Sequence]&, {iterators}]]];

	Monitor[
		ParallelTable[tableIndex++;expr, iterators, Evaluate[FilterRules[{opts}, Options[ParallelTable]]]]
		,
		If[OptionValue[ProgressTable, {opts}, "ShowInfo"],
			DetailedIndicator[tableIndex, tuplesLength, startTime, OptionValue[ProgressTable, {opts}, "Label"]]
			,
			DefaultIndicator[tableIndex, tuplesLength]
		]
	]
];


(* ::Chapter:: *)
(*NestApply*)


(* ::Text:: *)
(*Since functions like NestWhile accumulate all the calculations they can be memory expensive, and sometimes the result it's only used to be evaluated in another function. These nesting functions evaluate g while nesting with f.*)
(**)
(*Equivalences are*)
(**)
(*NestApplyList[f, g, expr, n] \[DoubleLeftRightArrow] Map[g, NestList[f, expr, n]]*)
(**)
(*and*)
(**)
(*NestApplyWhileList[f, g, expr, test] \[DoubleLeftRightArrow] Map[g, NestWhileList[f, expr, test]]*)
(**)
(*The best use case for this is when running a NestList in which {expr, f[expr], ... } returns very large expressions but {g[expr], g[f[expr]], ...} do not.*)


SetAttributes[Reaped, HoldFirst];
Reaped[list_] := First[Last[Reap[list]]];
Push[expr_, elem_] := Rest[Append[expr, elem]];

SetAttributes[NestApplyList, HoldAll];
NestApplyList[f_, g_, expr_, n_?NonNegative] := Reaped[Sow[g[Nest[(Sow[g[##]];f[##])&, expr, n]]]];

SetAttributes[NestApplyWhileList, HoldAll];
NestApplyWhileList[f_, g_, expr_, test_, All] := Map[g, NestWhileList[f, expr, test, All]];
NestApplyWhileList[f_, g_, expr_, test_, m:(_?Positive): 1] := 
Block[{testArg, tmp, iterations}, 
	(* This couldn't be implemented based on NestWhile because NestWhile saves the result at every step. Not very memory efficent. *)
	Reaped[
		testArg = NestList[(Sow[g[#]];f[#])&, expr, m-1];
			Sow[g[Last[testArg]]];

		While[Apply[test, testArg], 
			tmp = f[Last[testArg]];
			testArg = Push[testArg, tmp];
			Sow[g[tmp]];
		]
	]
];

NestApplyWhileList[f_, g_, expr_, test_, m_?Positive, max_?Positive] := 
Block[{testArg, tmp, iterations = 0}, 
	Reaped[
		testArg = NestList[(Sow[g[#]];f[#])&, expr, m-1];
		Sow[g[Last[testArg]]];

		While[Apply[test, testArg] && (iterations<max), 
			tmp = f[Last[testArg]];
			testArg = Push[testArg, tmp];
			Sow[g[tmp]];
			iterations++;
		]
	]
];

SetAttributes[NestListIndexed,HoldFirst];
NestListIndexed[f_, expr_, n_?NonNegative, startIndex_:1] := 
	Transpose[{Range[startIndex, n+startIndex], NestList[f, expr, n]}];


(* ::Chapter:: *)
(*Special evaluation*)


(* ::Text:: *)
(*EvaluateOnce and EvaluateIfChanged work by creating a special secret symbol that is checked for changes every evaluation. LinkParallelEvaluate is useful to do parallel computations in a clean way when using Mathlink or WSTP (I only tested it with Mathlink). Since sometimes one needs to execute also a WSTP function in the master kernel, LinkEvaluate helps to keep the master kernel definition out of the subkernels way.*)


SetAttributes[{EvaluateIfChanged, EvaluateOnce, SpecialSymbol}, {HoldAll, HoldAll, HoldAll}];

SpecialSymbol[expr_] := SpecialSymbol[expr] = Module[{t, tempSymbol}, SpecialSymbol[expr, t := Unique[tempSymbol]] = t];

EvaluateIfChanged[expr_, trackedSymbols_]:=
If[UnsameQ[SpecialSymbol[expr], trackedSymbols],
	SpecialSymbol[expr] = trackedSymbols;
	Return[ReleaseHold[expr]];
];

EvaluateOnce[expr_] := EvaluateIfChanged[expr, True];

SetAttributes[LinkParallelEvaluate, HoldFirst];
LinkParallelEvaluate[expr_ /; MemberQ[Names["*Parallel*"], ToString[Head[Unevaluated[expr]]]], name_String] := 
Module[{link, output},
	ParallelEvaluate[link = Install[name]];
	output = Evaluate[expr];
	ParallelEvaluate[Uninstall[link]];
	Return[output];
];

SetAttributes[LinkEvaluate, HoldFirst];
LinkEvaluate[expr_, name_String] := Module[{link, output},
	link = Install[name];
	output = Evaluate[expr];
	Uninstall[link];
	Return[output];
];


(* ::Chapter:: *)
(*End of package*)


End[ ]

EndPackage[ ]
