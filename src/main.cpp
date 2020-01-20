#include <iostream>
#include <string>

#include "parser/Parser.h"
#include "scheduler/Block.h"
#include "scheduler/Foreman.h"
#include "scheduler/Operator.h"



template <typename T>
void taskPushWorkOrders(Operator<T>& anOperator) {
    anOperator.pushWorkOrders();
}



int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: build/src/CS764_lichengxi_huang <mode>" << std::endl;
        std::cout << "To run the parser (EXPLAIN QUERY PLAN): mode=1" << std::endl;
        std::cout << "To run the scheduler:                   mode=2" << std::endl;
        exit(1);
    }

    if (strcmp(argv[1], "1") == 0) {
        const char* filename = "src/test.db";
        // const char* filename = "src/TPC-H.db";

        /// ***** test.db *****
        // const char *sql = "EXPLAIN QUERY PLAN select S.A from S where S.B=10;";
        const char *sql = "EXPLAIN QUERY PLAN select R.B, S.B from R, S where R.A=S.A and S.B > 5 and S.B < 10;";

        /// ***** TPC-H.db *****
        // const char *sql = "EXPLAIN QUERY PLAN select * from region where r_name = 'EUROPE'";
        // const char *sql = "EXPLAIN QUERY PLAN select p_partkey from part where p_size = 15";
        // const char *sql = "EXPLAIN QUERY PLAN select * from part, partsupp where p_partkey = ps_partkey";
        // const char *sql = "EXPLAIN QUERY PLAN select p_partkey from part, partsupp where p_partkey = ps_partkey and p_size = 15";
        // const char *sql = "EXPLAIN QUERY PLAN select s_acctbal, s_name, n_name, p_partkey, p_mfgr, s_address, s_phone, s_comment from part, supplier, partsupp, nation, region where p_partkey = ps_partkey and s_suppkey = ps_suppkey and p_size = 15 and p_type like '%BRASS' and s_nationkey = n_nationkey and n_regionkey = r_regionkey and r_name = 'EUROPE' and ps_supplycost = ( select min(ps_supplycost) from partsupp, supplier, nation, region where p_partkey = ps_partkey and s_suppkey = ps_suppkey and s_nationkey = n_nationkey and n_regionkey = r_regionkey and r_name = 'EUROPE' ) order by s_acctbal desc, n_name, s_name, p_partkey limit 100;";
        // const char *sql = "EXPLAIN QUERY PLAN CREATE TABLE COMPANY(ID INT PRIMARY KEY NOT NULL, NAME TEXT NOT NULL, AGE INT NOT NULL, ADDRESS CHAR(50),SALARY REAL);";
        // const char *sql = "EXPLAIN QUERY PLAN INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) VALUES (1, 'Paul', 32, 'California', 20000.00 );";

        Parser parser(sql);
        parser.parseQuery(filename);
        std::cout << parser.toString() << std::endl;



    } else if (strcmp(argv[1], "2") == 0) {
        unsigned n_cores = std::thread::hardware_concurrency();
        typedef Block T;
        auto[workOrderTx, workOrderRx] = channel<WorkOrder<T>>();
        auto[queue1Tx, queue1Rx] = channel<T>();
        auto[queue2Tx, queue2Rx] = channel<T>();
        auto[queue3Tx, queue3Rx] = channel<T>();
        auto[queue4Tx, queue4Rx] = channel<T>();

        for (int i = 0; i < 50; i++) {
            queue1Tx->send(new T(i));
        }
        queue1Tx->free();

        TableReference tableReference(workOrderTx->clone(), queue1Rx, queue2Tx);
        Select select(workOrderTx->clone(), queue2Rx, queue3Tx);
        Project project(workOrderTx->clone(), queue3Rx, queue4Tx);
        workOrderTx->free();

        std::thread t1(taskPushWorkOrders<T>, std::ref(tableReference));
        std::thread t2(taskPushWorkOrders<T>, std::ref(select));
        std::thread t3(taskPushWorkOrders<T>, std::ref(project));
        Foreman foreman(workOrderRx, n_cores);

        t1.join();
        t2.join();
        t3.join();
        foreman.join();


        while (true) {
            auto block = queue4Rx->receive();
            if (block == std::nullopt)
                break;
            std::cout << "output\t" << block.value()->toString() << std::endl;
        }

    } else {
        std::cout << "Usage: build/src/CS764_lichengxi_huang <mode>" << std::endl;
        std::cout << "To run the parser (EXPLAIN QUERY PLAN): mode=1" << std::endl;
        std::cout << "To run the scheduler:                   mode=2" << std::endl;
        exit(1);
    }

    return 0;
}
