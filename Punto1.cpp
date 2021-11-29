#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <vector>

//Macorc
#define lli long long int

using namespace std;

vector<string> memory;
// vector<vector<string>> process_information;
int memory_size;

bool worst_adjustment(lli, string);
bool best_adjustment(lli, string);
bool first_adjustment(lli, string);

bool solicitud(string process, lli cant_space, string strategy);

bool liberacion(string process);

void compactar(void);

void informe_estado(void);

void imprimir(void);

int main(int argc, char* argv[]) {
    int a;
    bool flag = true;
    string command, process, strategy;
    lli cant_space;
    if(argc == 3){ sscanf(argv[2], "%llu", &memory_size); }
    else{
	    fprintf(stderr, "Usage: m [numero]\n");
	    exit(1);
    }
    memory = vector<string>(memory_size, "");
    while(flag) {
        printf("Punto1/> ");
        cin >> command;

        if(command == "RQ") {
            cin >> process >> cant_space >> strategy;
            if(!solicitud(process, cant_space, strategy)) {
                cout << "Error storing the memory" << endl;
            }
        }
        else if(command == "RL") {
            cin >> process;
            if(!liberacion(process)) {
                cout << "Error setting free the memory" << endl;
            }
        }
        else if(command == "C") compactar();
        else if(command == "STAT") informe_estado();
        else if(command == "X") flag = false;
        else if(command == "L") imprimir();
        else if(command == "CLS") system("cls");
        else {
            cout << "Error: command not recognize :(" << endl;
        }
    }
    return 0;
}

void informe_estado() {
    lli i = 0;
    int j = 0;
    int contador = 0;
    string process;
    while(i < memory_size) {
        process = memory[i];
        while(memory[i] == process && i < memory_size) { ++i; ++contador; }
        cout << "Addresses [" << j + 1 << ":" << j+contador << "] ";
        if(process == "") { cout << "No utilizado" << endl; }
        else { cout << "Proceso " << process << endl; }
        j = i;
        contador = 0;
    }
}

bool liberacion(string process){
    bool ans = false;
    for(lli i = 0; i < memory_size; ++i){
        if(memory[i] == process){
            ans = true;
            memory[i] = "";
        }
    }
    return ans;
}

bool solicitud(string process, lli cant_space, string strategy){
    bool ans = true;
    if(strategy == "F"){ ans = first_adjustment(cant_space, process); }
    else if(strategy == "B"){ ans = best_adjustment(cant_space, process); }
    else if(strategy == "W"){ ans = worst_adjustment(cant_space, process); }
    else { ans = false; }
    return ans;
}

bool first_adjustment(lli cant_space, string process){
    bool flag = true;
    int j = -1;
    lli i, conteo = 0;
    for(i = 0; i < memory_size && flag; ++i){
        if(memory[i] == ""){
            if(j == -1) j = i;
            conteo++;
            if(conteo == cant_space){ flag = false; }
        }
        else { 
            j = -1; 
            conteo = 0;
        }
    }
    if(!flag){
        for(i = 0; i < cant_space; ++i){ memory[j+i] = process; }
    }
    return !flag;
}

void imprimir(){
    for(lli i = 0; i < memory_size; ++i){
        cout << i+1 << ": " << memory[i] << endl;
    }
}

bool best_adjustment(lli cant_space, string process) {
    bool ans = false;
    int cant = 0, pos = -1, min_cant = memory_size, min_pos = 0;
    for(lli i = 0; i <= memory_size; ++i) {
        if(i != memory_size && memory[i] == ""){
            if(pos == -1) pos = i;
            ++cant;
        }
        else {
            if(cant < min_cant && cant >= cant_space) {
                min_cant = cant;
                min_pos = pos;
            }
            cant = 0;
            pos = -1;
        }
    }
    if(min_cant >= cant_space) {
        int j = 0;
        while(j < cant_space) {
            memory[min_pos + j] = process;
            ++j;
        }
        ans = true;
    }
    return ans;
}

bool worst_adjustment(lli cant_space, string process){
    lli hueco = 0, contador = 0;
    lli j = -1, ini = 0;
    for(lli i = 0; i <= memory_size; ++i){
        if(i < memory_size && memory[i] == ""){
            if(j == -1) j = i;
            ++contador; 
        }
        else{
            if(contador > hueco){
                ini = j;
                hueco = contador;
            }
            j = -1;
            contador = 0;
        }
    }
    if(hueco >= cant_space){
        for(lli i = 0; i < cant_space; ++i){ memory[ini+i] = process; }
    }
    return hueco >= cant_space;
}

void compactar(void){
    string process = "";
    lli conteo = 0, i = 0;
    while(i <= memory_size){
        if(i < memory_size && memory[i] != "" && process == "") { 
            process = memory[i];
            conteo = 1; 
        }
        else if(i < memory_size && memory[i] == process && process != ""){ ++conteo; }
        else if(process != ""){
           liberacion(process);
           first_adjustment(conteo, process);
           conteo = 0;
           process = "";
           if(i != memory_size) i--; 
        }
        i++;
    }
}