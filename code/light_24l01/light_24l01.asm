//Deviec:FT61F02X
//-----------------------Variable---------------------------------
//-----------------------Variable END---------------------------------
		ORG		0000H
		LJUMP 	7FEH 			//0000 	3FFE
		ORG		07FDH
		LJUMP 	00H 			//07FD 	3800
		CLRR 	STATUS 			//07FE 	0103
		LJUMP 	7FDH 			//07FF 	3FFD
			END
