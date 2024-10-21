#include <unistd.h>

// Config settings
#include "Config.h"

// Alerts
#include "AlertUtils.h"

// Menu headers
#include "Menu/Menu.h"

// Util Headers
#include "Util/Bundle.hpp"
#include "Util/Macros.h"

// Il2CPP header
#include "Util/Resolver/IL2CPP_Resolver.hpp"

// Cheat Handle
#include "Core/Cheat.h"

namespace Tweak {
    void start() {

        //bool isOn = [switches isSwitchOn:APEncrypt("Switch Name Goes Here")];
        // int userValue = [[switches getValueFromSwitch:APEncrypt("Switch Name Goes Here")] intValue];
        // float userValue2 = [[switches getValueFromSwitch:APEncrypt("Switch Name Goes Here")] floatValue];

        // here add menu options!
        [menu addSwitch:APEncrypt("Masskill") //Switch without offset patching
            description:APEncrypt("Teleport all enemies to you without them knowing")
        ];

        [menu addOffsetSwitch:APEncrypt("One Hit Kill") //Switch with offset
            description:APEncrypt("Enemy will die instantly")
            offsets: {
                IGSecretHexInt("0x1001BB2C0"),
                IGSecretHexInt("0x1002CB3B0"),
                IGSecretHexInt("0x1002CB3B8")
            }
            bytes: {
                IGSecretHexData("0x00E0BF12C0035FD6"),
                IGSecretHexData("0xC0035FD6"),
                IGSecretHexData("0x00F0271E0008201EC0035FD6")
            }
        ];

        [menu addSliderSwitch:APEncrypt("Custom Move Speed") //Slider switch 
            description:APEncrypt("Set your custom move speed")
            minimumValue:0
            maximumValue:10
            sliderColor:UIColorFromHex(0x730130)
        ];

        [menu addTextfieldSwitch:APEncrypt("Custom Gold") //Text Field switch
            description:APEncrypt("Here you can enter your own gold amount")
            inputBorderColor:UIColorFromHex(0x730130)
        ];

        /* FOR ADVANCED USER | UNCOMMENT IT IF YOU KNOW HOW TO USE! */
        // Cheat::handle(); // here add your hook and stuff

        // Cheat::Init_Resolver(); // init il2cpp resolver
        // IL2CPP::Callback::Initialize(); // init callback
        // IL2CPP::Callback::OnUpdate::Add(Cheat::handle); //set the cheat handle

    }
}

namespace Initialize {


    void createMenu() {

    APMenu = [[Menu alloc] initWithTitle:APEncrypt("Made for @@SITE@@")
                            titleColor:[UIColor whiteColor]
                            titleFont:APEncrypt("Serif")
                                credits:APEncrypt(CREDITS)
                            headerColor:UIColorFromHex(0x99053A)
                        switchOffColor:UIColorFromHex(0xBF0D43)
                        switchOnColor:UIColorFromHex(0x99053A)
                        switchTitleFont:APEncrypt("Serif")
                        switchTitleColor:[UIColor whiteColor]
                        infoButtonColor:UIColorFromHex(0x99053A)
                    maxVisibleSwitches:3 
                            menuWidth:250
                                menuIcon:@MENU_ICON
                            menuButton:@MENU_BUTTON];

        Tweak::start();
        
    }

    static void canWriteWelcomeMessage(CFNotificationCenterRef center, void *observer,
                                CFStringRef name, const void *object,
                                CFDictionaryRef info) {
        timer(5) {
            SCLAlertView *alert = [[SCLAlertView alloc] initWithNewWindow];

            alert.customViewColor = [UIColor colorWithRed:0.16 green:0.40 blue:0.75 alpha:1.0];
            alert.showAnimationType = SCLAlertViewShowAnimationSlideInFromTop;
            alert.hideAnimationType = SCLAlertViewHideAnimationSlideOutToBottom;
            [alert addButton:APEncrypt("Visit iOSGods.com!") actionBlock:^(void) {
                [[UIApplication sharedApplication] openURL:[NSURL URLWithString:APEncrypt("https://iosgods.com/forum/13-free-jailbroken-cydia-cheats/")]];
                timer(2) {
                    createMenu(); 
                    });
                }];

            [alert addButton:APEncrypt("Thank you!") actionBlock:^(void) {
                timer(2) {
                    createMenu(); 
                    });
                }];

            alert.shouldDismissOnTapOutside = NO;

            [alert showSuccess:nil
                        subTitle:APEncrypt(WELCOME_MESSAGE)
                closeButtonTitle:nil
                        duration:99999999.0f];
        });
    }
    void init() {
        CFNotificationCenterAddObserver(CFNotificationCenterGetLocalCenter(), NULL, &canWriteWelcomeMessage, (CFStringRef)UIApplicationDidFinishLaunchingNotification, NULL, CFNotificationSuspensionBehaviorDeliverImmediately);
    }
} 

__attribute__((constructor)) static void onLoad() {
  dispatch_queue_t queue =
      dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
  dispatch_async(queue, ^{
    Initialize::init();
  });
}