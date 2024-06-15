#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


void helpScreen();
int MMU(int vAddress,int verbose);
void dump(int pageFaults,int r0,int r1,int r2,int r3,int r4,int r5,int r6,int r7);
void textbook();
void page_fault(int vAddress, int index);
int* convertBinary(int n);
int convertDecimal(int bArray[], int bits);

int main(){

	//while user has not typed "q"
	char input[30];
	int verboseOn = 0; //returns 1 for true and 0 for false
	int pAddress;
	int r0 = 0;
	int r1 = 0;
	int r2 = 0;
	int r3 = 0;
	int r4 = 0;
	int r5 = 0;
	int r6 = 0;
	int r7 = 0;
	// int num = 8196; // Example number
    // int *binaryArray = convertBinary(num);

	int pageFaults = 0;

	while(strcmp(input,"q")!=0){

		printf("mmu > ");
		scanf("%s",&input);

		if(strcmp(input,"?")==0){
			//if user types "?"
			helpScreen();
		}
		else if(strcmp(input, "dump") == 0){
			dump(pageFaults,r0,r1,r2,r3,r4,r5,r6,r7);
		}
		else if(strcmp(input,"on")==0){
			verboseOn = 1;
		}
		else if(strcmp(input,"off")==0){
			verboseOn = 0;
		}
		else if(strcmp(input,"textbook")==0){
			printf("\ntextbook setup\n");//confused

		}
		else{
			if(strcmp(input,"q")==0){

			}
			//convert to integer
			int vAddress = atoi(input);

			//set to pAddress
			pAddress = MMU(vAddress,verboseOn);
			if(pAddress == 8){
				pageFaults +=1;
			}
			//values of refCNT
			else if(pAddress = 0){
				r0 +=1;
			}
			else if(pAddress = 1){
				r1 +=1;
			}
			else if(pAddress = 2){
				r2 +=1;
			}
			else if(pAddress = 3){
				r3 +=1;
			}
			else if(pAddress = 4){
				r4 +=1;
			}
			else if(pAddress = 5){
				r5 +=1;
			}
			else if(pAddress = 6){
				r6 +=1;
			}
			else{
				r7 +=1;
			}

		}
	}
}

//convert decminal to binary
int* convertBinary(int dec) {
	//make memory for array
	int *binaryNum = (int *)malloc(16 * sizeof(int));
	//counter
	int i = 0; 
	if(dec < 0){
		//most significant bit
		dec = -dec;
		binaryNum[15] = 1;
	} 
	else{
		binaryNum[15] = 0;
	}
	//all other bits sets to 0
	for (int k = 0; k < 15; k++) {
		binaryNum[k] = 0;
	}
	while(dec>0){
		//remainder put into array
		binaryNum[i]=dec%2;
		dec=dec/2;
		i++;
	} 
	// Return the binary array
	return binaryNum;
} 

//convert array to decimal from binary
int convertDecimal(int bArray[], int bits){
	int decimal = 0;
	int power = 1;//power of 2

	//array in reverse order
	for (int i = bits - 1; i >= 0; i--) {
		//current digit to power of 2
		decimal += bArray[i] * power;
		//power of two increased
		power *= 2;
	}
	return decimal;
}

void dump(int pageFaults,int r0,int r1,int r2,int r3,int r4,int r5,int r6,int r7){
	printf("Virtual Address      Physical Address");
	printf("\nIndex   Maps-To       Index  RefCnt");
	printf("\n15         X");
	printf("\n14         X");
	printf("\n13         X");
	printf("\n12         X");
	printf("\n11         X");
	printf("\n10         X");
	printf("\n 9         X");
	printf("\n 8         X");
	printf("\n 7         8            7      %d",r7);
	printf("\n 6         8            6      %d",r6);
	printf("\n 5         3            5      %d",r5);
	printf("\n 4         4            4      %d",r4);
	printf("\n 3         0            3      %d",r3);
	printf("\n 2         6            2      %d",r2);
	printf("\n 1         1            1      %d",r1);//trouble here
	printf("\n 0         2            0      %d",r0);
	printf("\n Number of Page Faults: %d\n",pageFaults);
}

void page_fault(int vAddress,int index){//trouble figuring out page faults, ran out of time
	printf("\nAddress: %d  offset: %d  blkNum: %d  PageTableLoc: %d",vAddress,4090,index,8);
	printf("\nPageFrame 0 had a reference count of 0. Evicted from Page Table");
	printf("\nVirtual Page table at location %d set to 0 (unmapped)",index);
	printf("\nAfter-Trap -- Address: %d offset: %d blkNum: %d PageTableLoc %d",vAddress,4090,index,0);
	printf("\n%d -->  %d\n",vAddress,4090);
}


//returns pAddress
int MMU(int vAddress,int verbose){
	//convert the address to binary
    int *binaryArray = convertBinary(vAddress);

    //reverse array
    for (int i = 0; i < 16/2; i++)
    {
        int temp = binaryArray[i];
        binaryArray[i] = binaryArray[16 - 1 - i];
        binaryArray[16 - 1 - i] = temp;
    }

	//first 4 bits of virtual address
	int fourBits[4] = {binaryArray[0],binaryArray[1],binaryArray[2],binaryArray[3]};

	//first 4 bits to integer
	int index = convertDecimal(fourBits,4);

	// printf("\nIndex of %d: %d\n",vAddress, index);


	//holds 3 bits to add to 12 bit offset
	int threeBits[3];
	//holds 12 offset bits
	int twelveBits[12];
	//add 12 offset bits to twelveBits array
	for(int i =4; i<16;i++){
		twelveBits[i-4] = binaryArray[i];//ADDING TO THE FINAL
	}

    //holds physicalAddress
    int physicalAddressBinary[15];
    //holds physical address decimal
    int physicalAddress;

    if(index == 6 || index == 7 || index == 8 || index == 10 || index == 12 || index == 13 || index == 14 || index == 15){
    	//page fault
    	page_fault(vAddress,index);
    	return 8;
    }
    else if(index == 0){
    	//three bits

    	int threeBits[3] = {0,1,0};

	    //add 3 bits
	    for(int i =0;i<3;i++){
	    	physicalAddressBinary[i] = threeBits[i];//ADDING TO THE FINAL
	    }

	    //add 12 bits
	    for(int i = 3;i<15;i++){
	    	physicalAddressBinary[i] = twelveBits[i-3];//ADDING TO THE FINAL
	    }
	    //convert to decimal
	    physicalAddress = convertDecimal(physicalAddressBinary,15);

		if(verbose == 1){
			printf("\nAddress: %d  offset: %d  blkNum: %d  PageTableLoc: %d\n",vAddress,convertDecimal(twelveBits,12),0,2);
			printf("%d -->  %d\n",vAddress,physicalAddress);
		}
		else{
			printf("Physical Address: %d\n",physicalAddress);
		}

	    return 2;
    }
    else if(index == 1){
    	int threeBits[3] = {0,0,1};
	    //add 3 bits
	    for(int i =0;i<3;i++){
	    	physicalAddressBinary[i] = threeBits[i];//ADDING TO THE FINAL
	    }

	    //add 12 bits
	    for(int i = 3;i<15;i++){
	    	physicalAddressBinary[i] = twelveBits[i-3];//ADDING TO THE FINAL
	    }
	    physicalAddress = convertDecimal(physicalAddressBinary,15);
	    if(verbose == 1){
			printf("\nAddress: %d  offset: %d  blkNum: %d  PageTableLoc: %d\n",vAddress,convertDecimal(twelveBits,12),1,1);
			printf("%d -->  %d\n",vAddress,physicalAddress);
		}
		else{
			printf("Physical Address: %d\n",physicalAddress);
		}
	    return 1;
    }
    else if(index == 2){
    	
    	int threeBits[3] = {1,1,0};

	    //add 3 bits
	    for(int i =0;i<3;i++){
	    	physicalAddressBinary[i] = threeBits[i];//ADDING TO THE FINAL
	    }

	    // printf("PRINT ADDRESS WITH 3 BITS\n");
	    // for(int i = 0; i<15;i++){
	    // 	printf("%d",physicalAddressBinary[i]);
	    // }

	    //add 12 bits
	    for(int i = 3;i<15;i++){
	    	physicalAddressBinary[i] = twelveBits[i-3];//ADDING TO THE FINAL
	    }
	    physicalAddress = convertDecimal(physicalAddressBinary,15);
	    if(verbose == 1){
			printf("\nAddress: %d  offset: %d  blkNum: %d  PageTableLoc: %d\n",vAddress,convertDecimal(twelveBits,12),2,6);
			printf("%d -->  %d\n",vAddress,physicalAddress);
		}
		else{
			printf("Physical Address: %d\n",physicalAddress);
		}

	    // printf("PRINT ADDRESS WITH ALL BITS\n");
	    // for(int i = 0; i<15;i++){
	    // 	printf("%d",physicalAddressBinary[i]);
	    // }

	    return 6;
    }
    else if(index == 3){
    	int threeBits[3] = {0,0,0};
	    //add 3 bits
	    for(int i =0;i<3;i++){
	    	physicalAddressBinary[i] = threeBits[i];//ADDING TO THE FINAL
	    }

	    //add 12 bits
	    for(int i = 3;i<15;i++){
	    	physicalAddressBinary[i] = twelveBits[i-3];//ADDING TO THE FINAL
	    }
	    physicalAddress = convertDecimal(physicalAddressBinary,15);
	    if(verbose == 1){
			printf("\nAddress: %d  offset: %d  blkNum: %d  PageTableLoc: %d\n",vAddress,convertDecimal(twelveBits,12),3,0);
			printf("%d -->  %d\n",vAddress,physicalAddress);
		}
		else{
			printf("Physical Address: %d\n",physicalAddress);
		}

	    return 0;
    }
    else if(index == 4){
    	int threeBits[3] = {1,0,0};
	    //add 3 bits
	    for(int i =0;i<3;i++){
	    	physicalAddressBinary[i] = threeBits[i];//ADDING TO THE FINAL
	    }

	    //add 12 bits
	    for(int i = 3;i<15;i++){
	    	physicalAddressBinary[i] = twelveBits[i-3];//ADDING TO THE FINAL
	    }
	    physicalAddress = convertDecimal(physicalAddressBinary,15);
	    if(verbose == 1){
			printf("\nAddress: %d  offset: %d  blkNum: %d  PageTableLoc: %d\n",vAddress,convertDecimal(twelveBits,12),4,4);
			printf("%d -->  %d\n",vAddress,physicalAddress);
		}
		else{
			printf("Physical Address: %d\n",physicalAddress);
		}

	    return 4;
    }
    else if(index == 5){
    	int threeBits[3] = {0,1,1};
	    //add 3 bits
	    for(int i =0;i<3;i++){
	    	physicalAddressBinary[i] = threeBits[i];//ADDING TO THE FINAL
	    }

	    //add 12 bits
	    for(int i = 3;i<15;i++){
	    	physicalAddressBinary[i] = twelveBits[i-3];//ADDING TO THE FINAL
	    }
	    physicalAddress = convertDecimal(physicalAddressBinary,15);
	    if(verbose == 1){
			printf("\nAddress: %d  offset: %d  blkNum: %d  PageTableLoc: %d\n",vAddress,convertDecimal(twelveBits,12),5,3);
			printf("%d -->  %d\n",vAddress,physicalAddress);
		}
		else{
			printf("Physical Address: %d\n",physicalAddress);
		}

	    return 3;
    }

    else if(index == 9){
    	int threeBits[3] = {1,0,1};
	    //add 3 bits
	    for(int i =0;i<3;i++){
	    	physicalAddressBinary[i] = threeBits[i];//ADDING TO THE FINAL
	    }

	    //add 12 bits
	    for(int i = 3;i<15;i++){
	    	physicalAddressBinary[i] = twelveBits[i-3];//ADDING TO THE FINAL
	    }
	    physicalAddress = convertDecimal(physicalAddressBinary,15);
	    if(verbose == 1){
			printf("\nAddress: %d  offset: %d  blkNum: %d  PageTableLoc: %d\n",vAddress,convertDecimal(twelveBits,12),9,5);
			printf("%d -->  %d\n",vAddress,physicalAddress);
		}
		else{
			printf("Physical Address: %d\n",physicalAddress);
		}

	    return 5;
    }

    else if(index == 11){
    	// printf("HERE");
    	int threeBits[3] = {1,1,1};
	    //add 3 bits
	    for(int i =0;i<3;i++){
	    	physicalAddressBinary[i] = threeBits[i];//ADDING TO THE FINAL
	    }

	    //add 12 bits
	    for(int i = 3;i<15;i++){
	    	physicalAddressBinary[i] = twelveBits[i-3];//ADDING TO THE FINAL
	    }
	    physicalAddress = convertDecimal(physicalAddressBinary,15);
	    if(verbose == 1){
			printf("\nAddress: %d  offset: %d  blkNum: %d  PageTableLoc: %d\n",vAddress,convertDecimal(twelveBits,12),11,7);
			printf("%d -->  %d\n",vAddress,physicalAddress);
		}
		else{
			printf("Physical Address: %d\n",physicalAddress);
		}

	    return 7;
    }

    else{
    	printf("Something went wrong");
    	return 0;
    }
    // printf("here");
    //clear memory
	free(binaryArray);

}
void helpScreen(){

	printf("Commands:\n     q         Quit the program\n     ?         Print this help screen\n     on        Turns Verbose Mode On\n     off       Turns Verbose Mode Off\n     dump      Dumps the current Page Table\n     textbook  Sets up pages as in Fig 3-9 in text\n     1234      (Or any number -- decode this address)\n");
}