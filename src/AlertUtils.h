#pragma once

#include "Includes/SCLAlertView/SCLAlertView.h"

#define ___ALERT_TITLE "@@APPNAME@@ Cheats | @@SITE@@"

namespace Alert {    
    void showWaiting(NSString *msg, SCLAlertView* __strong *alert);
    void dismisWaiting(SCLAlertView *al);

    void showSuccess(NSString *msg);
    void showInfo(NSString *msg, float duration);
    void showError(NSString *msg);
}