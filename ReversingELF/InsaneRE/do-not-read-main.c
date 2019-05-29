#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>

struct fam{
	int flag_first_13[13];
	int abc_junk[13];
} harry;

struct alam{
	int flag_end[6];
	char junk[20];
} styles;

struct dingDong{
    struct fam james;
    struct alam charles;
} gotem;

struct dingDong init_dingDong(int flag_first_13_[13], int flag_end_[6]){
    struct fam fam_ = {flag_first_13_, {10,20,30,40,50,60,70,8,9,10,11,12,13}};
    struct alam alam_ = {flag_end_, "Flag{NOTTHIS}"};
    struct dingDong init_this = {fam_, alam_};
    
    return init_this;
}

void check_flag(){
    int flag_first[13] = {1,2,3,4,5,6,7,8,9,10,11,12,13};  // binary stuff (first 13 numbers)
    int flag_end[6] = {1,2,3,4,5,6};                       // binary stuff (next 6 numbers)
    
    int junk_1[13] = {66, 76, 120, 7, 27, 156, 85, 98, 37, 236, 43, 211, 54};
    int junk_2[6]  = {34, 254, 172, 68, 46};
    
	struct dingDong theWitchDead = init_dingDong(flag_first, flag_end);
	char junkCheck[20] = {207, 8, 134, 120, 249, 218, 156, 162, 135, 97, 234, 241, 43, 66, 187, 104, 148, 191, 36};
	
	char real_flag[20];
	char false_flag[20];
	char false_flag_2[20];
	for(int i=0; i<20; ++i){
		if (i < 13){ 
			real_flag[i] = theWitchDead.james.flag_first_13[i];
			false_flag[i] = theWitchDead.james.flag_first_13[i]^0x69;
			false_flag_2[i] = theWitchDead.james.flag_first_13[i]^0x96;
		}else{
			int xyz = i - 13;
			int jkp = i + 13;
			real_flag[xyz] = theWitchDead.charles.flag_end[xyz];
			false_flag[xyz] = theWitchDead.james.flag_first_13[jkp]^0x96;
			false_flag_2[xyz] = theWitchDead.james.flag_first_13[xyz]^0x69;
		}
	}
	if(ptrace(PTRACE_TRACEME, 0, 1, 0) == -1){
		puts("No debuggers :DD");
		exit(2);
	    
	}
	
	for (int i=0; i<20; ++i){
		real_flag[i] ^= 0x6c;
	}
	
	
	for(int i=0; i<20; ++i){
		int sum = 0;
		for(int c=0; c<8; ++c){
			int current_bit = (int)(junkCheck[i] & (1 << c));
			if (!(c&1))
				sum += !current_bit;
			else
				sum += current_bit;
			
		}
		
		junkCheck[i] = sum;
	}
	if ( strcmp(&junkCheck, &real_flag) )
		puts(":)");

}

int main(){
	puts("Flag is L0lr3ktn0t7h15");
	
	if(ptrace(PTRACE_TRACEME, 0, 1, 0) == -1){
		puts("No debuggers allowed :P");
		return 2;
	}
	return 0;
}
