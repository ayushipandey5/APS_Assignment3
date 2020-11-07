#include <vector>
#include <fstream>
#include <istream>
#include <iostream>
#include <string>
#include <climits>
#include <cmath>
#include <cstring>
#include <stdio.h>
#include <string.h>


# define ll long long
# define SIZE 1000000

using namespace std;

ll CHUNK[SIZE] = { 0 };

typedef struct Node {
    ll element;
    ll fileName;
}node;


ll fileSize(const char* filename) {
    FILE* fh = fopen(filename, "rb");
    fseek(fh, 0, SEEK_END);
    long long size = ftell(fh);
    fclose(fh);

    return (size);
}

FILE* openFile(char* fileName, char* mode)
{
    FILE* fp = fopen(fileName, mode);
    if (fp == NULL) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    return fp;
}


void merge(ll* a, ll l, ll m, ll h) {
    ll b[h + 1];
    ll i = l;
    ll j = m + 1;
    ll k = l;

    while (i <= m && j <= h) {
        if (a[i] > a[j]) {
            b[k] = a[j];
            k += 1;
            j += 1;
        }
        else {
            b[k] = a[i];
            k += 1;
            i += 1;

        }
    }
    while (i <= m) {
        b[k] = a[i];
        k += 1;
        i += 1;
    }

    while (j <= h) {
        b[k] = a[j];
        k += 1;
        j += 1;
    }

    for (ll i = l; i <= h; i++) {
        a[i] = b[i];
    }
}


void mergesort(ll* a, ll l, ll h) {
    if (l >= h) {
        return;
    }
    ll m = (l + h) / 2;
    mergesort(a, l, m);
    mergesort(a, m + 1, h);
    merge(a, l, m, h);
}

void swapNode(node* t1, node* t2)
{
    node temp;
    temp = *t1;
    *t1 = *t2;
    *t2 = temp;
}


void heapify(node* a, ll n, ll i) {
    ll l = 2 * i + 1;
    ll r = 2 * i + 2;
    ll sm = i;
    if (l < n && a[l].element < a[i].element)
        sm = l;
    if (r < n && a[r].element < a[sm].element)
        sm = r;
    if (sm != i) {
        swapNode(&a[i], &a[sm]);
        heapify(a, n, sm);
    }
}

void buildHeap(node* a, ll n) {
    for (ll i = (n - 1) / 2; i >= 0; i--) {
        heapify(a, n, i);
    }
}



int main(int argc, char** argv) {
    if (argc < 3) {
        cout << "Enter valid number of arguments\n";
    }
    else {
        char* input_file_name, * output_file_name;
        input_file_name = argv[1];
        output_file_name = argv[2];
        ll size = fileSize(input_file_name);
        ll partitionSize = 1e4;
        ll numPartition = ceil(size / partitionSize);
        long long totalNumbers = 0;

        // cout<<input_file_name<<"  "<<output_file_name<<endl;
        // cout<<size<<endl;



/*--------------------------------Sort EACH FILE----------------------------*/


        ifstream in;
        in.open(input_file_name, std::ios_base::in);
        bool chunkEmpty = true;
        ll counter = 0;
        ll fileNum = 0;
        while (1) {
            string number;
            ll i = 0;
            while (i < SIZE && getline(in, number, ',')) {
                ll num = stoll(number);
                chunkEmpty = false;
                CHUNK[i] = num;
                totalNumbers++;

                i += 1;
            }

            if (chunkEmpty)
                break;

            mergesort(CHUNK, 0, i - 1);
            ofstream out;
            string partNum = to_string(fileNum);
            out.open(partNum + ".txt", ios::out);

            for (ll j = 0; j < i; j++)
            {
                string del = ",";
                out << CHUNK[j] << del;
            }
            chunkEmpty = true;
            fileNum++;
        }

        /*---------------------------------MERGING FILES------------------------------*/

        ifstream partitions[fileNum];
        ofstream finalOutput;
        finalOutput.open(output_file_name, ios::out);

        node heap[fileNum];

        for (ll i = 0; i < fileNum; i++) {
            partitions[i].open(to_string(i) + ".txt", ios::in);
            string number;
            if (getline(partitions[i], number, ',')) {
                ll num;
                num = stoll(number);
                heap[i].element = num;
                heap[i].fileName = i;
            }
            else {
                cout << " Error line 197" << endl;
                break;
            }

        }
        // for(ll i=0;i<fileNum;i++){   
        //     string number;
        //     getline(partitions[i],number,',');
        //     cout<<number<<"   "<< i<<"   endl  ";
        //     ll num;
        //     if(!number.empty()){
        //         num = stoll(number);
        //         heap[i].element = num;
        //         heap[i].fileName = i;
        //     }
        // }

        buildHeap(heap, fileNum);

        // for(ll j = 0;j<fileNum;j++)
        //     cout<<heap[j].element<<"    "<<heap[j].fileName<<endl;

        counter = 0;


        // // finalOutput<<"yauzaa";

        while (1) {
            if (counter == totalNumbers) {
                break;
            }
            string del = ",";
            finalOutput << heap[0].element << del;
            ll index = heap[0].fileName;
            string number;
            if (getline(partitions[index], number, ','))
                heap[0].element = stoll(number);
            else {
                heap[0].element = LLONG_MAX;
                heap[0].fileName = -1;
            }
            heapify(heap, fileNum, 0);
            counter++;
        }




    }//argc else end


    return 0;
}
