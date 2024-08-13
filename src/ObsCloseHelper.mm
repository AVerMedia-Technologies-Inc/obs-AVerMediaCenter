#include "ObsCloseHelper.h"
#import <Foundation/Foundation.h>
#include <AppKit/AppKit.h>

namespace ObsCloseHelper{
  void close(QMainWindow*) {
          @autoreleasepool {
              //// 要終止的應用程式的 Bundle Identifier
              //NSString *bundleIdentifierToTerminate = @"com.obsproject.obs-studio";
			  // How to get bundle ID?
			  // https://stackoverflow.com/a/39400658
			  NSString *bundleIdentifierToTerminate = [[NSBundle mainBundle] bundleIdentifier];
			  NSLog(@"My bundle id: %@", bundleIdentifierToTerminate);

              // 取得所有正在運行的應用程式
              NSArray *runningApplications = [NSRunningApplication runningApplicationsWithBundleIdentifier:bundleIdentifierToTerminate];

              // 確認找到了目標應用程式
              if ([runningApplications count] > 0) {
                  // 取得第一個符合的應用程式
                  NSRunningApplication *targetApplication = runningApplications[0];

                  // 終止應用程式
                  [targetApplication terminate];
                  NSLog(@"Terminated: %@", bundleIdentifierToTerminate);
              } else {
                  NSLog(@"Unable to find: %@", bundleIdentifierToTerminate);
              }
          }
  }
}
