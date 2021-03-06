static const Reserved r_chars[] = {
	/* reserved char */		/* replace with */
	{"<",				"[lt]"},
	{">",	 			"[gt]"},
	{":",	 			"[cl]"},
	{"\"",				"[qt]"}, 
	{"/",	 			"[fs]"},
	{"\\", 				"[bs]"},
	{"|",	 			"[vb]"},
	{"?",	 			"[qm]"},
	{"*",				"[ak]"},
};

static const Reserved r_names[] = {
	/* reserved name */		/* replace with */
	{"CON",				"_CON_"},
	{"PRN", 			"_PRN_"},
	{"AUX", 			"_AUX_"},
	{"NUL", 			"_NUL_"},
	{"COM1",			"_COM1_"},
	{"COM2",			"_COM2_"},
	{"COM3",			"_COM3_"},
	{"COM4",			"_COM4_"},
	{"COM5",			"_COM5_"},
	{"COM6",			"_COM6_"},
	{"COM7",			"_COM7_"},
	{"COM8",			"_COM8_"},
	{"COM9",			"_COM9_"},
	{"LPT1",			"_LPT1_"},
	{"LPT2",			"_LPT2_"},
	{"LPT3",			"_LPT3_"},
	{"LPT4",			"_LPT4_"},
	{"LPT5",			"_LPT5_"},
	{"LPT6",			"_LPT6_"},
	{"LPT7",			"_LPT7_"},
	{"LPT8",			"_LPT8_"},
	{"LPT9",			"_LPT9_"},
};
