import os
import time
import urllib.request
import json

def main():
    print("--- برنامه مدیریت زمان هوشمند پایتون شروع به کار کرد ---")

    gh_pat = os.getenv("GH_PAT")
    repo = os.getenv("GITHUB_REPOSITORY")
    ref = os.getenv("GITHUB_REF_NAME", "main")

    can_restart = True
    if not gh_pat or not repo:
        print("==========================================================")
        print(" هشدار: متغیر GH_PAT یا GITHUB_REPOSITORY یافت نشد!")
        print(" سیستم ری‌استارت خودکار غیرفعال است، اما ویندوز روشن می‌ماند...")
        print("==========================================================")
        can_restart = False

    # کل زمان انتظار: ۵.۵ ساعت (۱۹۸۰۰ ثانیه)
    total_seconds = 19800
    # هر ۶۰۰ ثانیه (۱۰ دقیقه) یک پیام چاپ می‌کنیم تا گیت‌هاب سرور را به دلیل بی‌فعالی خاموش نکند
    interval = 600 
    elapsed = 0

    print(f"سرور ویندوز با موفقیت آماده استفاده است. زمان‌سنج ۵.۵ ساعته فعال شد.")
    
    while elapsed < total_seconds:
        time.sleep(interval)
        elapsed += interval
        remaining_minutes = (total_seconds - elapsed) // 60
        print(f"[سیستم فعال است] {remaining_minutes} دقیقه تا اجرای مجدد خودکار باقی مانده است...")

    if can_restart:
        print("زمان خواب به پایان رسید. در حال ارسال درخواست به گیت‌هاب برای اجرای مجدد...")
        
        url = f"https://api.github.com/repos/{repo}/actions/workflows/keep-alive.yml/dispatches"
        headers = {
            "Accept": "application/vnd.github+json",
            "Authorization": f"Bearer {gh_pat}",
            "X-GitHub-Api-Version": "2022-11-28",
            "User-Agent": "KeepAlive-App"
        }
        data = json.dumps({"ref": ref}).encode("utf-8")

        try:
            req = urllib.request.Request(url, data=data, headers=headers, method="POST")
            with urllib.request.urlopen(req) as response:
                if response.status == 204:
                    print("درخواست با موفقیت ارسال شد! این سرور به زودی خاموش شده و سرور جدید شروع به کار می‌کند.")
                else:
                    print(f"پاسخ غیرمنتظره از گیت‌هاب: {response.status}")
        except Exception as e:
            print(f"خطا در ارسال درخواست به API گیت‌هاب: {e}")

if __name__ == "__main__":
    main()
