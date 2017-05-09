#include <sys/shm.h>
#include <unistd.h> 
#include <string>

//#include "gtest/gtest.h"
#include "qconf_shm.h"
#include "qconf_format.h"
#include "qlibc.h"
#include "qconf_log.h"
#define MAX_SLOT_COUNT (800000)

using namespace std;

int main(int argc, char const *argv[])
{	
	static qhasharr_t *tbl;
    static const key_t shmkey = 12345;

    int rel = create_hash_tbl(tbl, shmkey, 0666);
    string key("hello"), val("11111"), val_out;
    
    //EXPECT_EQ(QCONF_OK, retCode);
    //EXPECT_STREQ(val_new.data(), val_out.data());
    while(1)
    {
    	int retCode = hash_tbl_get(tbl, key, val_out);
    	sleep(5);
    }
	return 0;
}