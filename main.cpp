#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <cstdlib>

int main() {
    std::cout << "--- برنامه مدیریت زمان گیت هاب اکشن شروع به کار کرد ---" << std::endl;

    const char* gh_pat_env = std::getenv("GH_PAT");
    const char* repo_env = std::getenv("GITHUB_REPOSITORY");
    const char* ref_env = std::getenv("GITHUB_REF_NAME");

    bool can_restart = true;

    // سیستم هوشمند: اگر سکرت ست نشده باشد، سرور را خاموش نکن! فقط هشدار بده.
    if (!gh_pat_env || std::string(gh_pat_env).empty() || !repo_env) {
        std::cerr << "==========================================================" << std::endl;
        std::cerr << " هشدار: متغیر GH_PAT در سکرت های گیت هاب یافت نشد!" << std::endl;
        std::cerr << " سیستم ری استارت خودکار غیرفعال شد، اما ویندوز روشن می ماند..." << std::endl;
        std::cerr << "==========================================================" << std::endl;
        can_restart = false;
    }

    // زمان خواب ۵.۵ ساعت (۱۹۸۰۰ ثانیه)
    const int wait_seconds = 19800; 
    std::cout << "سرور ویندوز با موفقیت اماده استفاده است. به مدت ۵.۵ ساعت روشن می ماند..." << std::endl;

    // انتظار برای زنده نگه داشتن رانر
    std::this_thread::sleep_for(std::chrono::seconds(wait_seconds));

    // اگر توکن موجود بود، ریکوئست ری‌استارت را بفرست
    if (can_restart) {
        std::string gh_pat = gh_pat_env;
        std::string repo = repo_env;
        std::string ref = ref_env ? ref_env : "main";

        std::cout << "در حال ارسال درخواست اجرای مجدد به گیت‌هاب..." << std::endl;

        std::string curl_command = "curl -X POST "
                                   "-H \"Accept: application/vnd.github+json\" "
                                   "-H \"Authorization: Bearer " + gh_pat + "\" "
                                   "-H \"X-GitHub-Api-Version: 2022-11-28\" "
                                   "https://api.github.com/repos/" + repo + "/actions/workflows/keep-alive.yml/dispatches "
                                   "-d \"{\\\"ref\\\":\\\"" + ref + "\\\"}\"";

        int result = std::system(curl_command.c_str());
        if (result == 0) {
            std::cout << "درخواست اجرای مجدد با موفقیت ارسال شد!" << std::endl;
        } else {
            std::cerr << "خطا در ارسال درخواست به گیت‌هاب!" << std::endl;
        }
    }

    return 0;
}
