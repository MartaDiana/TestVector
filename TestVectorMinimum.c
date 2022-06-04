#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

/* Variables */

int out1, out2;
int numNodes = 40; 
short inputNodes[256][40];
short arrayInput[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
short faultFinal[40] = {999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999};
short vectorFinal[40] = {999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999};


/* End of Variables */

/* Fungsi Pembantu Minimisasi */

//Mencari banyak elemen dari array test yang tidak terdapat pada array total
int countDiff(short* test, short* total, short id_test, short id_total) {                     
    int count = 0;
    int j = 0;

    for (int i = 0; i < id_test; i++){
        if (test[i] == 0) continue;
        for (j = 0; j < id_total; j++){
            if ((test[i] * i) == total[j]) break;
        }
        if (j == id_total) count += 1;
    }

    return count;
}

//Menguji apabila suatu nilai tes sudah terdaftar dalam array penyimpanan nilai tes
unsigned char notInArray(short num, short* arr, short len) {
    for (int i = 0; i < len; i++) {
        if (num == arr[i]) return 0;
    }
    return 1;
}

//Memasukkan isi array 1 secara unik ke array 2
void arrayMerge(short* src, short* dest, short id_src, short* id_dest){
    for (int i = 0; i < id_src; i++) {
        if (src[i] == 0) continue;
        if (notInArray(src[i] * i, dest, *id_dest) ) {
            dest[*id_dest] = src[i] * i;
            *id_dest = *id_dest + 1;
        }
    }
}

//Mencetak isi array
void printArray(short* arr, int size){
    int i = 0;
    printf("[");
    for (i = 0; i < size - 1; i++) {
        printf("%d, ", arr[i]);
    }
    printf("%d]", arr[i]);
}

/* Fungsi untuk Gate */ 

int andGate (int a, int b) { // fungsi gerbang and
    return a && b; 
}

int orGate (int a, int b) { // fungsi gerbang or 
    return a || b;
}

int nandGate (int a, int b) { // fungsi gerbang nand
    return !(a&&b);
}

int norGate(int a, int b){ // fungsi gerbang nor
    return !(a||b);
}

int notGate(int a){ // fungsi gerbang not
    return !a;
}

/* End of Fungsi untuk Gate */



/* Main function */



void main () {
    short plcFault = 0;
    short numFault = 0;
    short locFault = 0;
    short id_fault = 0;
    short id_vector = 0;

    for (int i = 0; i < 256; i++){
        for (int j = 0; j < numNodes; j++) inputNodes[i][j] = 0;
    }

    printf("A | B | C | D | E | F | G | H |A/0|A/1|B/0|B/1|C/0|C/1|D/0|D/1|E/0|E/1|F/0|F/1|G/0|G/1|H/0|H/1|I/0|I/1|J/0|J/1|K/0|K/1|L/0|L/1|M/0|M/1|N/0|N/1|O/0|O/1|P/0|P/1|Q/0|Q/1|R/0|R/1|Z00|Z01/Z10/Z11/ou1/ou2 \n");
    printf("________________________________________________________________________________________________________________________________________________________________________________________________________\n");
    
    for ( int a = 0;  a < 2;  a++)
    {
        for (int b = 0; b < 2; b++)
        {
            for (int c = 0; c < 2; c++)
            {
                for (int d = 0; d < 2; d++)
                {
                    for (int e = 0; e < 2; e++)
                    {
                        for (int f = 0; f < 2; f++)
                        {
                            for (int g = 0; g < 2; g++)
                            {
                                for (int h = 0; h < 2; h++)
                                {                                         
                                    printf("%d | %d | %d | %d | %d | %d | %d | %d |", a, b, c, d, e, f, g, h);
                                            
                                    // deklarasi output for good circuit 
                                    out1 = norGate(andGate(a,b),nandGate(c,d)); //Z0
                                    out2 = andGate(
                                        orGate(
                                            notGate(norGate(andGate(a,b),nandGate(c,d))), 
                                            andGate(orGate(e,f), notGate(g))
                                        ), 
                                        notGate(h)
                                    ); //Z1
                                    // end of deklarasi output for good circuit 
                                            
                                    int row = a*128 + b*64 + c*32 + d*16 + e*8 + f*4 + g*2 + h; 
                                            
                                    //Iterasi per node (propagasi)
                                    for (int i = 0; i < numNodes; i++)
                                        {  
                                            arrayInput[0] = a; arrayInput[1] = b; arrayInput[2] = c; arrayInput[3] = d; 
                                            arrayInput[4] = e; arrayInput[5] = f; arrayInput[6] = g; arrayInput[7] = h; 
                                            arrayInput[8] = andGate(a,b); //i
                                            arrayInput[9] = nandGate(c,d); //j
                                            arrayInput[10] = orGate(e,f); //k
                                            arrayInput[11] = notGate(g); //l
                                            arrayInput[12] = norGate(andGate(a,b), nandGate(c,d)); //m
                                            arrayInput[13] = andGate(orGate(e,f), notGate(g)); //n
                                            arrayInput[14] = norGate(andGate(a,b), nandGate(c,d)); //o
                                            arrayInput[15] = notGate(norGate(andGate(a,b), nandGate(c,d))); //p
                                            arrayInput[16] = notGate(h); //q
                                            arrayInput[17] = orGate(
                                                notGate(norGate(andGate(a,b), nandGate(c,d))),
                                                andGate(orGate(e,f), notGate(g))
                                            ); //r
                                            arrayInput[18] = norGate(andGate(a,b),nandGate(c,d)); //Z0
                                            arrayInput[19] = andGate(
                                                orGate(
                                                    notGate(norGate(andGate(a,b),nandGate(c,d))), 
                                                    andGate(orGate(e,f), notGate(g))), 
                                                notGate(h)
                                            ); //Z1

                                            arrayInput[i/2] = i % 2; //ekspresi 1 / 0 

                                            // propagasi pada gate level 1
                                            int outputSA1 = norGate(andGate(arrayInput[0],arrayInput[1]),nandGate(arrayInput[2],arrayInput[3]));
                                            int outputSA2 = andGate(
                                                            orGate
                                                                (
                                                                notGate(norGate(andGate(arrayInput[0],arrayInput[1]),
                                                                nandGate(arrayInput[2],arrayInput[3]))), 
                                                                andGate(orGate(arrayInput[4],arrayInput[5]), notGate(arrayInput[6]))
                                                                ), 
                                                            notGate(arrayInput[7])
                                                            );
                                            // propagasi pada gate level 2 
                                            int outputSA1_2 = norGate(arrayInput[8],arrayInput[9]);
                                            int outputSA2_2 = andGate(
                                                orGate(
                                                    notGate(norGate(arrayInput[8],arrayInput[9])), 
                                                    andGate(arrayInput[10], arrayInput[11])), 
                                                notGate(arrayInput[7])
                                            );
                                            // propagasi pada gate level 3 
                                            int outputSA1_3 = arrayInput[12];
                                            int outputSA2_3 = andGate(
                                                orGate(
                                                    notGate(arrayInput[12]), 
                                                    arrayInput[13]), 
                                                notGate(arrayInput[7])
                                            );
                                            // propagasi pada gate level 4
                                            int outputSA1_4 = arrayInput[12];
                                            int outputSA2_4 = andGate(
                                                orGate(
                                                    arrayInput[15], 
                                                    arrayInput[13]), 
                                                notGate(arrayInput[7])
                                            );
                                            // propagasi pada gate level 5
                                            int outputSA1_5 = arrayInput[12];
                                            int outputSA2_5 = andGate(arrayInput[17], arrayInput[16]);
                                            // propagasi pada gate percabangan
                                            // pada node O
                                            int outputSA1_o = norGate(andGate(a,b),nandGate(c,d));
                                            int outputSA2_o = andGate(
                                                orGate(
                                                    notGate(arrayInput[14]), 
                                                    andGate(orGate(e,f), notGate(g))), 
                                                notGate(h)
                                            );
                                            // output Z0 dan Z1
                                            int outputSA1_z = arrayInput[18];
                                            int outputSA2_z = arrayInput[19];

                                            //printf("%d/%d|%d/%d ", out1, outputSA1, out2, outputSA2);   
                                            if ((outputSA1 != out1) || (outputSA2 != out2)) {
                                                inputNodes[row][i] = 1;
                                            }  
                                            if ((outputSA1_2 != out1) || (outputSA2_2 != out2)) {
                                                inputNodes[row][i] = 1;
                                            }  
                                            if ((outputSA1_3 != out1) || (outputSA2_3 != out2)) {
                                                inputNodes[row][i] = 1;
                                            } 
                                            if ((outputSA1_4 != out1) || (outputSA2_4 != out2)) {
                                                inputNodes[row][i] = 1;
                                            } 
                                            if ((outputSA1_5 != out1) || (outputSA2_5 != out2)) {
                                                inputNodes[row][i] = 1;
                                            } 
                                            if ((outputSA1_o != out1) || (outputSA2_o != out2)) {
                                                inputNodes[row][i] = 1;
                                            }
                                            if ((outputSA1_z != out1) || (outputSA2_z != out2)) {
                                                inputNodes[row][i] = 1;
                                            }  
                                                                                    
                                            char benar[10] = "v";
                                            char salah[10] =" ";

                                            if(inputNodes[row][i] == 1){
                                                printf(" %s |", benar);
                                            }
                                            else {
                                                printf(" %s |", salah);
                                            }

    
                                            }
                                        printf("\t%d %d %d\n",out1, out2, row);
                                                            
                                }  
                            }
                            
                        }
                        
                    }
                    
                }
            }
        }
    }  
    
    
    /* Minimisasi tes */
    //Menghasilkan 2 array yang berisi test vector yang menghasilkan fault coverage minimum pada titik tes ke-i
    //printf("\nTest Vector Minimum: " );
    printf ("\nPENGUJIAN TEST VECTOR MINIMUM\n");
    printf("\nLine\t| Unique Total Fault\t| Total Fault Coverage\t| Test Vector \n");
    while ((id_fault < 40) && (id_vector < 256)) {
        numFault = 0;
        for (int i = 0; i < 256; i++) {
            if (notInArray(i, (short*) &vectorFinal, id_vector)) {
                plcFault = countDiff((short*) &inputNodes[i], (short*) &faultFinal, sizeof(inputNodes[i]) >> 1, id_fault);
                if (plcFault > numFault){
                    numFault = plcFault;
                    locFault = i;
                }
            }
        }
        arrayMerge((short*) &inputNodes[locFault], (short*) &faultFinal, sizeof(inputNodes[locFault]) >> 1, &id_fault);
        vectorFinal[id_vector] = locFault;
        id_vector += 1;
        printf("\n%d\t| %d\t\t\t| %d\t\t\t|", locFault, numFault, id_fault);
        
        int decimal, remainder, basis=1, biner = 0;
        decimal = locFault;
        while (locFault>0)
        {
            remainder = locFault%2;
            biner = biner + remainder*basis;
            locFault = locFault/2;
            basis=basis*10;
        }
       printf(" %08d\n", biner);   
    }
    
    printf("\nTotal Test Vector\t: %d", id_vector);
    printf("\nFault Coverage\t\t: ");
    if(id_fault == 40) {
        printf("100%%\n");
    }
    else {
        printf("<100%%\n");
    }
    
    //printf("\n\nFaults: ");printArray(&faultFinal, id_fault);
    //printf("\nVectors: ");printArray(&vectorFinal, id_vector);



    }
