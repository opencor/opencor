#ifdef SERIALIZATIONSTART
__SERIALIZATIONSTART = DIAG_START_SERIALIZATION,
#undef SERIALIZATIONSTART
#endif

DIAG(err_fe_not_a_pch_file, CLASS_ERROR, diag::MAP_ERROR, "input is not a PCH file: '%0'", 0, true, false, false, false, 0)
DIAG(err_fe_pch_error_at_end_block, CLASS_ERROR, diag::MAP_FATAL, "error at end of module block in PCH file: '%0'", 0, true, false, false, false, 0)
DIAG(err_fe_pch_file_modified, CLASS_ERROR, diag::MAP_FATAL, "file '%0' has been modified since the precompiled header was built", 0, true, false, false, false, 0)
DIAG(err_fe_pch_malformed, CLASS_ERROR, diag::MAP_FATAL, "malformed or corrupted PCH file: '%0'", 0, true, false, false, false, 0)
DIAG(err_fe_pch_malformed_block, CLASS_ERROR, diag::MAP_FATAL, "malformed block record in PCH file: '%0'", 0, true, false, false, false, 0)
DIAG(err_fe_unable_to_read_pch_file, CLASS_ERROR, diag::MAP_ERROR, "unable to read PCH file: '%0'", 0, true, false, false, false, 0)
DIAG(err_not_a_pch_file, CLASS_ERROR, diag::MAP_FATAL, "'%0' does not appear to be a precompiled header file", 0, true, false, false, false, 0)
DIAG(err_pch_langopt_mismatch, CLASS_ERROR, diag::MAP_ERROR, "%0 was %select{disabled|enabled}1 in PCH file but is currently %select{disabled|enabled}2", 0, true, false, false, false, 0)
DIAG(err_pch_langopt_value_mismatch, CLASS_ERROR, diag::MAP_ERROR, "%0 differs in PCH file vs. current file", 0, true, false, false, false, 0)
DIAG(err_pch_with_compiler_errors, CLASS_ERROR, diag::MAP_ERROR, "PCH file contains compiler errors", 0, true, false, false, false, 0)
DIAG(note_pch_macro_defined_as, CLASS_NOTE, diag::MAP_FATAL, "definition of macro '%0' in the precompiled header", 0, true, false, false, false, 0)
DIAG(note_using_macro_def_from_pch, CLASS_NOTE, diag::MAP_FATAL, "using this macro definition from precompiled header", 0, true, false, false, false, 0)
DIAG(warn_cmdline_conflicting_macro_def, CLASS_ERROR, diag::MAP_ERROR, "definition of the macro '%0' conflicts with the definition used to build the precompiled header", 0, true, false, false, false, 0)
DIAG(warn_cmdline_missing_macro_defs, CLASS_WARNING, diag::MAP_WARNING, "macro definitions used to build the precompiled header are missing", 0, true, false, false, false, 0)
DIAG(warn_macro_name_used_in_pch, CLASS_ERROR, diag::MAP_ERROR, "definition of macro %0 conflicts with an identifier used in the precompiled header", 0, true, false, false, false, 0)
DIAG(warn_pch_compiler_options_mismatch, CLASS_ERROR, diag::MAP_ERROR, "compiler options used when building the precompiled header differ from the options used when using the precompiled header", 0, true, false, false, false, 0)
DIAG(warn_pch_different_branch, CLASS_ERROR, diag::MAP_ERROR, "PCH file built from a different branch (%0) than the compiler (%1)", 0, true, false, false, false, 0)
DIAG(warn_pch_target_triple, CLASS_ERROR, diag::MAP_ERROR, "PCH file was compiled for the target '%0' but the current translation unit is being compiled for target '%1'", 0, true, false, false, false, 0)
DIAG(warn_pch_version_too_new, CLASS_ERROR, diag::MAP_ERROR, "PCH file uses a newer PCH format that cannot be read", 0, true, false, false, false, 0)
DIAG(warn_pch_version_too_old, CLASS_ERROR, diag::MAP_ERROR, "PCH file uses an older PCH format that is no longer supported", 0, true, false, false, false, 0)
