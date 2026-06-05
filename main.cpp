#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <cstdlib>

// این برنامه به زبان C++ نوشته شده است تا پس از اجرای ویندوز، زمان را مدیریت کرده
// و پس از ۵.۵ ساعت با استفاده از توکن امنیتی، اکشن را مجدداً تریگر کند.

int main() {
    std::cout << "--- برنامه مدیریت زمان گیت هاب اکشن شروع به کار کرد ---" << std::endl;

    // دریافت متغیرهای محیطی از گیت‌هاب اکشن
    const char* gh_pat_env = std::getenv("GH_PAT");
    const char* repo_env = std::getenv("GITHUB_REPOSITORY");
    const char* ref_env = std::getenv("GITHUB_REF_NAME");

    if (!gh_pat_env || !repo_env) {
        std::cerr << "خطا: متغیرهای محیطی GH_PAT یا GITHUB_REPOSITORY یافت نشدند!" << std::endl;
        return 1;
    }

    std::string gh_pat = gh_pat_env;
    std::string repo = repo_env;
    std::string ref = ref_env ? ref_env : "main";

    // محاسبه زمان ۵.۵ ساعت به ثانیه (۵ ساعت و ۳۰ دقیقه)
    // 5.5 * 3600 = 19800 ثانیه
    const int wait_seconds = 19800; 

    std::cout << "سیستم به مدت ۵.۵ ساعت به خواب می‌رود تا رانر فعال بماند..." << std::endl;
    std::cout << "زمان خواب فعال شد. پس از اتمام این زمان، درخواست اجرای مجدد ارسال خواهد شد." << std::endl;

    // متوقف کردن برنامه به مدت مشخص شده
    std::this_thread::sleep_for(std::chrono::seconds(wait_seconds));

    std::cout << "زمان خواب به پایان رسید. در حال ارسال درخواست اجرای مجدد به گیت‌هاب..." << std::endl;

    // ساخت دستور curl برای فعال‌سازی مجدد اکشن (Workflow Dispatch)
    // این دستور به API گیت‌هاب درخواست می‌دهد تا ورک‌فلو را دوباره اجرا کند
    std::string curl_command = "curl -X POST "
                               "-H \"Accept: application/vnd.github+json\" "
                               "-H \"Authorization: Bearer " + gh_pat + "\" "
                               "-H \"X-GitHub-Api-Version: 2022-11-28\" "
                               "https://api.github.com/repos/" + repo + "/actions/workflows/keep-alive.yml/dispatches "
                               "-d \"{\\\"ref\\\":\\\"" + ref + "\\\"}\"";

    // اجرای دستور سیستم
    int result = std::system(curl_command.c_str());

    if (result == 0) {
        std::cout << "درخواست اجرای مجدد با موفقیت ارسال شد!" << std::endl;
    } else {
        std::cerr << "خطا در ارسال درخواست به گیت‌هاب!" << std::endl;
    }

    std::cout << "پایان اجرای برنامه کمکی." << std::endl;
    return 0;
}
