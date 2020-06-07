#include <iostream>
#include "ConcurrentMap_Leapfrog.h"
#include<omp.h>

typedef junction::ConcurrentMap_Leapfrog<turf::u32, int*> LeapfrogMap;


void testMap() {
    LeapfrogMap lfmap;

    float startTime = omp_get_wtime();
    for(int i=2;i<2+10000000;i++)
    {
        int* value = new int(i);
        lfmap.assign(i, value);
    }
    float endTime = omp_get_wtime();
    std::cout << "Leapfrog write:" << endTime - startTime << std::endl;

    startTime = omp_get_wtime();
    for(int i=2;i<2+10000000;i++)
    {
        int* value = lfmap.get(i);
        *value = i*i;
        value = lfmap.erase(i);
        delete value;
    }
    endTime = omp_get_wtime();
    std::cout << "Leapfrog read:" << endTime - startTime << std::endl;
}

void testMap2() {
    LeapfrogMap lfmap;

    float startTime = omp_get_wtime();
    #pragma omp parallel for num_threads(4)
    for(int i=2;i<2+10000000;i++)
    {
        int* value = new int(i);
        lfmap.assign(i, value);
    }
    float endTime = omp_get_wtime();
    std::cout << "Leapfrog write with omp:" << endTime - startTime << std::endl;

    startTime = omp_get_wtime();
    #pragma omp parallel for num_threads(4)
    for(int i=2;i<2+10000000;i++)
    {
        int* value = lfmap.get(i);
        *value = i*i;
        value = lfmap.erase(i);
        delete value;
    }
    endTime = omp_get_wtime();
    std::cout << "Leapfrog read with omp:" << endTime - startTime << std::endl;
}

int main() 
{
    junction::QSBR::Context context = junction::DefaultQSBR.createContext();
    testMap();
    testMap2();
    junction::DefaultQSBR.update(context);
    junction::DefaultQSBR.destroyContext(context);

    return 0;
}