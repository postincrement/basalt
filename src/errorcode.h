
#ifndef ERRORCODE_H_
#define ERRORCODE_H_

enum ErrorCode 
{
  eInternalError                 = 0x0000,

  //
  // warnings
  //
  eWarning_First                 = 0x1000, 
  eWarning_PrintUsingQuestionMark = eWarning_First,
  eWarning_RemUsingQuote,
  eWarning_VarDefinedButNotUsed,
  eWarning_VarIsSynonym,
  eWarning_UnreachableCode,
  eWarning_Last                   = 0x7fff,

  //
  // errors
  //
  eError_First                   = 0x8000, 
  eError_Unknown = eError_First,
  eError_Parser,
  eError_GotoDestinationNotFound,
  Error_DuplicateLineNumber,
  Error_UndeclaredVariable,
  Error_MismatchedNext
};

#endif // ERRORCODE_H_
