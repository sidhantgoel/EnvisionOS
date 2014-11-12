struc iso9660vol_desc
	.reserved1						resb 8
	.sys_ident						resb 32
	.vol_ident						resb 32
	.reserved2						resb 8
	.total_sect						resb 8
	.reserved3						resb 32
	.vol_set_size					resb 4
	.vol_seq_nmbr					resb 4
	.sect_size						resb 4
	.path_table_size				resb 8
	.1st_sect_little_1st_table		resb 4
	.1st_sect_little_2nd_table		resb 4
	.1st_sect_big_1st_table			resb 4
	.1st_sect_big_2nd_table			resb 4
	.root_dir_rec					resb 34
	.vol_set_ident					resb 128
	.publish_ident					resb 128
	.data_preparer_ident			resb 128
	.app_ident						resb 128
	.copyright_file_ident			resb 37
	.abstract_file_ident			resb 37
	.bibliographical_file_ident		resb 37
	.date_time_vol_create			resb 17
	.date_time_mod					resb 17
	.date_time_vol_expires			resb 17
	.date_time_vol_effective		resb 17
	.reserved4						resb 2
	.reserved_app					resb 512
	.reserved5						resb 653
endstruc

;virtual at 0
;	iso9660vol_desc iso9660vol_desc
;end virtual

struc iso9660dir_desc
	.len_dir_rec					resb 1
	.ext_attr_rec_len				resb 1
	.loc_data						resb 8
	.data_len						resb 8
	.date_time						resb 7
	.file_flags						resb 1
	.file_unit_size					resb 1
	.interleave_gap_size			resb 1
	.vol_seq_nmbr					resb 4
	.len_file_ident					resb 1
	.file_ident						resb 0
endstruc

;virtual at 0
;	iso9660dir_desc iso9660dir_desc
;end virtual