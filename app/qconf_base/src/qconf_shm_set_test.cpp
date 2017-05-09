#include <sys/shm.h>

#include <string>
#include "qconf_log.h"
//#include "gtest/gtest.h"
#include "qconf_shm.h"
#include "qconf_format.h"
#include "qlibc.h"
#include <unistd.h> 
#define MAX_SLOT_COUNT (800000)

using namespace std;
int reul = 1;
int main(int argc, char const *argv[])
{	

	static qhasharr_t *tbl;
    static const key_t shmkey = 123456;

    reul = create_hash_tbl(tbl, shmkey, 0666);
    int retCode = 0;
    string key("hello"), val("11111"), val_out;
    string val_new(81, 'a');

    retCode = hash_tbl_set(tbl, key, val);
    //EXPECT_EQ(QCONF_OK, retCode);

    retCode = hash_tbl_get(tbl, key, val_out);
    //EXPECT_EQ(QCONF_OK, retCode);

    //EXPECT_STREQ(val_new.data(), val_out.data());
    printf("%s\n", val_out.data());


    while(1)
    {
    	sleep(10);
    }
	return 0;
}