#include <iostream>
#include <bitset>
#include <string>

using namespace std;


bitset<32> decimalToBinary(int number, int numBits) {
  
  if (number >= 0) {
      bitset<32> binary(number & ((1 << numBits) - 1));
      return binary;
    } else {
      bitset<32> binary((number *-1) & ((1 << numBits) - 1));
      binary.set(numBits - 1, 1);
      return binary;
    }
}
int binaryToDecimal(bitset<32> binary, int numBits) {
  
    if (binary[numBits - 1]) {
  
        return -((1 << numBits) - binary.to_ulong());
    } else {
       
        return binary.to_ulong();
    }
}
bitset<32> complementoDos(bitset<32> binary, int numBits) {   

    return (~binary | bitset<32>("1")).to_ulong() & ((1 << numBits) - 1);

}

bitset<32> sumar(bitset<32> a, bitset<32> b, int numBits) {
    bitset<32> suma = a.to_ulong() + b.to_ulong();
    return suma & std::bitset<32>((1 << numBits) - 1);
}

bitset<32> restar(bitset<32> a, bitset<32> b, int numBits) {
    return sumar(a, complementoDos(b, numBits), numBits);
}

bitset<32> convertBinaryNegative(bitset<32> &binary, int numBits) {
  bitset<32> binaryNegative = binary;
  bitset<1> MSBA(binary[numBits - 1]);

  if(MSBA[0] == 1) {
    binaryNegative[numBits - 1] = 0;
  }else {
    binaryNegative[numBits - 1] = 1;
  }
  return binaryNegative;
}

bitset<1> corrimientoAritmetico(bitset<32> &A, bitset<32> &Q, int numBits) {
    bitset<1> MSBA(A[numBits - 1]);
    bitset<1> LSBA(A[0]);
     // Corrimiento aritmético a la derecha en A

    A >>= 1;
    A.set(numBits - 1, MSBA[0]);
 
    bitset<1> LSBQ(Q[0]);
    Q >>= 1;
    Q.set(numBits - 1, LSBA[0]);


    return LSBQ;
}


bitset<32> boothMultiplication(bitset<32> &M, bitset<32> &Q,  bitset<32> C,  bitset<32> Mc, int numBits, int productoDecimal) {
    bitset<1> Q1(0);
    bitset<32> A(0);
    cout<<"Cuenta   |  "<<"            A                      |  "<<"                  Q                    |  "<<"  Q0  |  "<<" Q+ "<<endl;

    for (int i = numBits; i > 0; i--) {  
        cout<<" i = "<<i<<"   | "<<A<<"    |  "<<Q<<"       |  "<<" Q0: "<<Q[0]<<"|   "<<Q1<<endl;
        if (Q[0] < Q1[0]) {
            if(productoDecimal > 0){
              A = sumar(A, M, numBits);
              cout<<" M  ->   | "<<M<<endl;
              cout<<" A=A+M-> | "<<A<<endl;
            }else{
              A = sumar(A, Mc, numBits);
              cout<<" Mc  ->  | "<<Mc<<endl;
              cout<<" A=A+Mc->| "<<A<<endl;
            }

        }       
        else if (Q[0] > Q1[0]) {        
          if(productoDecimal > 0){
            A = restar(A, M, numBits);
            cout<<" Mc ->   | "<<complementoDos(M,numBits)<<endl;
          }else{
            A = sumar(A, C, numBits);
            cout<<" Mc ->   | "<<C<<endl;
          }    
            cout<<" A=A+Mc->| "<<A<<endl;
        }else{
            cout<<" A    -> | "<<A<<endl;
        }

      
        Q1 = corrimientoAritmetico(A, Q, numBits);
        cout<<"correr-> | "<<A<<"    |  "<<Q<<"       |  "<<" Q0: "<<Q[0]<<"|   "<<Q1<<endl;
           
       
        cout<<"----------------------------------------------------------"<<endl;
    }

    A &= ((1 << numBits) - 1); // A solo debe tener numBits de bits
    bitset<32> concatenatedAQ = (A.to_ulong() << numBits) | Q.to_ulong();


    if(productoDecimal < 0){
      //negativo
     
      bitset<32> resultado = complementoDos(concatenatedAQ, numBits * 2);
      resultado.set((numBits) * 2 - 1, 1);
      return resultado;
    } else {
      //positivo
      return concatenatedAQ;
    }
    
}



int main() {


    int multiplicandoDecimalM = 5;
    int multiplicadorDecimalQ = 3;
    int numBits = 8;

    cout<<"Ingrese el numero mayor Multiplicando (M) Ejm: -7 o 7: ";std::cin>>multiplicandoDecimalM;
    cout<<"Ingrese el numero menor Multiplicador (Q) Ejm: 3: ";std::cin>>multiplicadorDecimalQ;
    cout<<"Ingrese la cantidad de bits(4): ";std::cin>>numBits;cout<<"\n";
    
    int productoDecimal = multiplicandoDecimalM * multiplicadorDecimalQ;

    bitset<32> multiplicandoM = decimalToBinary(multiplicandoDecimalM, numBits);
    bitset<32> multiplicadorQ = decimalToBinary(multiplicadorDecimalQ, numBits);

    cout<<"*******************************************"<<endl;
    cout<<"M : "<<multiplicandoDecimalM<< " -> "<<multiplicandoM<<endl;
    cout<<"Q : "<<multiplicadorDecimalQ<< " -> "<<multiplicadorQ<<endl;
    cout<<"C: "<<multiplicandoDecimalM * -1<< " -> "<<convertBinaryNegative( multiplicandoM,numBits)<<endl;
    cout<<"Mc:   "<< " -> "<<complementoDos( convertBinaryNegative( multiplicandoM,numBits),numBits)<<endl;
    cout<<"*******************************************"<<endl;
    bitset<32> C = convertBinaryNegative( multiplicandoM,numBits);
    bitset<32> Mc = complementoDos(C, numBits);

    bitset<32> producto = boothMultiplication(multiplicandoM, multiplicadorQ, C, Mc, numBits, productoDecimal);
    // int productoDecimal = binaryToDecimal(producto, numBits);
   
    cout << "\nProducto: " << productoDecimal << " (binario: " << producto << ")" << "\n\n";

    return 0;
}
