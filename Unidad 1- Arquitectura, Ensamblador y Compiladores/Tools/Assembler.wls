#!/usr/bin/env wolframscript
(* ::Package:: *)

(* ::Title:: *)
(*Assembler*)


(* ::Input::Initialization:: *)
cpuMnemonics = {
"LoadA","LoadB","SetA","SetB","Store","Add",
"Substract","Increment","Decrement","Jump","JumpPos","JumpNeg","JumpZero","JumpNotZero","JumpOdd",
"Call","Return","Print","Draw"
};
opcodes = Thread[cpuMnemonics->Take[Tuples[{0,1},10],Length[cpuMnemonics]]];
DecimalToBinary[n_,size_ :10]:=PadLeft[IntegerDigits[n,2],size];

GetInstructions[asmcode_]:=Block[{separated,completed},
separated = DeleteCases[Map[StringSplit,StringSplit[asmcode,"\n"]],{}];
completed = ReplaceAll[separated,l_/;(Length[l]==1):>Append[l,0]];
Return[completed];
];
GetPosition[instructions_,token_]:=Block[{pos,labels,countingRules},
pos = Position[instructions,token];
If[Length[pos]>1,Return[$Failed]];
labels = Take[instructions,pos[[1,1]]-1][[All,1]];
countingRules = Join[Thread[cpuMnemonics->2],{"Label"->0,"Declare"->1}];

Total[ReplaceAll[labels,countingRules]]
];
ProcessTags[instructions_]:=Block[{labels,variablePos},
labels = Cases[instructions,{"Label",tag_}:>(tag->GetPosition[instructions,{"Label",tag}])];
variablePos = Cases[instructions,{"Declare",tag_,value_}:>(tag->GetPosition[instructions,{"Declare",tag,value}])];
Join[labels,variablePos]
];
MachineInstructions[instructions_]:=Block[{setNumeric,removeLabels,replaceTags},
setNumeric = ReplaceAll[instructions,{{"Declare",_,value_}:>ToExpression[value],{"SetA",value_}:>{"SetA",ToExpression[value]},{"SetB",value_}:>{"SetB",ToExpression[value]} }];
removeLabels = DeleteCases[setNumeric,{"Label",__}];
replaceTags = ReplaceAll[removeLabels,ProcessTags[instructions]];

Return[replaceTags];
];
AssemblyCompile[asmcode_]:=Block[{linearized,rules, machineCode},
linearized = Flatten[MachineInstructions[GetInstructions[asmcode]]];
rules = Append[opcodes,n_Integer:>DecimalToBinary[n]];
machineCode = ReplaceAll[linearized,rules];
Return[machineCode];
];

Assemble[inputFile_,outputFile_]:=Block[{code,machineCode,machineCodeAsNumbers,ba,file},
code = Import[inputFile,"Text"];
machineCode = AssemblyCompile[code];
machineCodeAsNumbers = Map[FromDigits[#,2]&,machineCode];
Export[outputFile,machineCodeAsNumbers,"UnsignedInteger16"];
];

If[Length[$ScriptCommandLine]==3,
Assemble[$ScriptCommandLine[[2]],$ScriptCommandLine[[3]]]
,
Print["Invalid number of arguments"]
];
