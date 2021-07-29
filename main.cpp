
#include "AiLog.h"
using namespace std;
using namespace aiheart;

int main()
{

Log::StartAiLogIo();

int count = 0;
while(true)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    count++;
    LOGD("Test Warn %d!!!", count);
    LOGI("Test Warn %d!!!", count);
    LOGW("Test Warn %d!!!", count);
    LOGE("Test Warn %d!!!", count);
}


Log::StopAiLogIo();



return 1;

}