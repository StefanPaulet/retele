#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <cstring>
#include <fstream>

void * threadMain ( void * param ) {

    std :: ifstream fin;
    fin.open ( "../resources/sport.txt" );
    char s1 [ 128 ];
    char s2 [ 128 ];
    char s3 [ 128 ];
    for ( int i = 0; i < 5; ++ i ) {
        fin.getline ( s1, 128 );
        if ( strlen ( s1 ) == 0 ) {
            fin.seekg ( 0, std :: ios_base :: beg );
            -- i;
            continue;
        }
        fin.getline ( s2, 128 );
        fin.getline ( s3, 128  );
        std :: cout << s1 << " sursa: " << s2 << " link: " << s3 << '\n';
        sleep ( 1 );
    }
    pthread_cancel ( pthread_self() );
    return nullptr;
}

int main() {

    std :: ifstream fin;
    fin.open ( "../resources/sport.txt" );
    char s1 [ 128 ];
    char s2 [ 128 ];
    char s3 [ 128 ];
    for ( int i = 0; i < 5; ++ i ) {
        fin.getline ( s1, 128 );
        if ( strlen ( s1 ) == 0 ) {
            fin.clear();
            fin.seekg ( 0, std :: ios_base :: beg );
            --i;
            continue;
        }
        fin.getline ( s2, 128 );
        fin.getline ( s3, 128  );
        std :: cout << s1 << " sursa: " << s2 << " link: " << s3 << '\n';
    }
}