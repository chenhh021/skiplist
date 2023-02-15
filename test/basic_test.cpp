//
// Created by chenh on 2023/1/27.
//
#include <gtest/gtest.h>
#include "../skiplist.h"
#include <iostream>

//only for stress test
#define NUM_THREADS 4
skiplist<int, std::string> list(16);

void *insert_helper(void* total_count){
    int total;
    total = (long long)total_count;
    int test_count = total / NUM_THREADS;
    for(int i = 0; i < test_count; ++i){
        list.insert_element(rand() % total, "a");
    }
    pthread_exit(NULL);
}

void *search_helper(void* total_count){
    int total;
    total = (long long)total_count;
    int test_count = total / NUM_THREADS;
    for(int i = 0; i < total; ++i){
        list.search_element(rand()%test_count);
    }
    pthread_exit(NULL);
}

void stress_test_helper(int total_count){
    srand(time(NULL));
    {
        pthread_t threads[NUM_THREADS];
        int rc, i;

        // insert stress test
        auto start = std::chrono::high_resolution_clock::now();

        for( i = 0; i < NUM_THREADS; ++i){
            rc = pthread_create(&threads[i], NULL, insert_helper, (void *)total_count);

            if (rc){
                std::cout << "create thread failed: " << rc << std::endl;
                exit(-1);
            }
        }

        void *ret;
        for( i = 0; i < NUM_THREADS; ++i ) {
            if (pthread_join(threads[i], &ret) !=0 )  {
                perror("pthread_create() error");
                exit(-2);
            }
        }
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        std::cout << "insert elapsed:" << elapsed.count() << std::endl;

        // search stress test
        start = std::chrono::high_resolution_clock::now();

        for( i = 0; i < NUM_THREADS; ++i){
            rc = pthread_create(&threads[i], NULL, search_helper, (void *)total_count);

            if (rc){
                std::cout << "create thread failed: " << rc << std::endl;
                exit(-1);
            }
        }

        for( i = 0; i < NUM_THREADS; ++i ) {
            if (pthread_join(threads[i], &ret) !=0 )  {
                perror("pthread_create() error");
                exit(-2);
            }
        }
        finish = std::chrono::high_resolution_clock::now();
        elapsed = finish - start;
        std::cout << "search elapsed:" << elapsed.count() << std::endl;
    }
}

TEST(basic_test, display_test){
    std::cout << "---------display_test----------" << std::endl;
    skiplist<int, std::string> skiplist(6);
    skiplist.insert_element(2, "two", true);
    skiplist.insert_element(3, "three", true);
    skiplist.insert_element(11, "eleven", true);
    skiplist.insert_element(13, "thirteen", true);
    skiplist.insert_element(17, "seventeen", true);
    skiplist.insert_element(19, "nineteen", true);
    skiplist.insert_element(19, "nineteen", true);

    std::cout << "skiplist size:" << skiplist.size() << std::endl;

    skiplist.dump();

    skiplist.load();

    skiplist.search_element(13);
    skiplist.search_element(18);


    skiplist.display();

    skiplist.delete_element(11);
    skiplist.delete_element(17);

    std::cout << "skiplist size:" << skiplist.size() << std::endl;

    skiplist.display();

    std::cout << "----------display test end----------" << std::endl;
}

TEST(basic_test, stress_test_insert){
    std::cout << "stress_test_start" <<std::endl;

    std::cout << "test_size: 100,000" <<std::endl;
    stress_test_helper(100000);

    std::cout << "test_size: 1,000,000" <<std::endl;
    stress_test_helper(1000000);

    std::cout << "test_size: 10,000,000" <<std::endl;
    stress_test_helper(10000000);
}

int main(int argc, char **argv) {
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}