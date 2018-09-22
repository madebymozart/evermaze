# Add project specific ProGuard rules here.
# By default, the flags in this file are appended to flags specified
# in E:\developSoftware\Android\SDK/tools/proguard/proguard-android.txt
# You can edit the include path and order by changing the proguardFiles
# directive in build.gradle.
#
# For more details, see
#   http://developer.android.com/guide/developing/tools/proguard.html

# Add any project specific keep options here:

# If your project uses WebView with JS, uncomment the following
# and specify the fully qualified class name to the JavaScript interface
# class:
#-keepclassmembers class fqcn.of.javascript.interface.for.webview {
#   public *;
#}

# Cocos2d-x
-keep public class org.cocos2dx.** { *; }
-dontwarn org.cocos2dx.**
-keep public class com.chukong.** { *; }
-dontwarn com.chukong.**
-keep public class com.huawei.android.** { *; }
-dontwarn com.huawei.android.**

# Facebook
-keep public class com.facebook.** { *; }
-dontwarn com.facebook.**

# Fmod
-keep class org.fmod.** { *; }
-dontwarn org.fmod.**

# Apache HTTP
-keep class org.apache.http.** { *; }
-dontwarn org.apache.http.**

# Google Play Services
-keep public class com.google.android.gms.** { public *; }
-dontwarn com.google.android.gms.**

# Firebase
-keep class com.google.firebase.** { *; }
-dontwarn com.google.firebase.**

-keep class * extends java.util.ListResourceBundle {
    protected Object[][] getContents();
}

-keep public class com.google.android.gms.common.internal.safeparcel.SafeParcelable {
    public static final *** NULL;
}

-keepnames @com.google.android.gms.common.annotation.KeepName class *
-keepclassmembernames class * {
    @com.google.android.gms.common.annotation.KeepName *;
}

-keepnames class * implements android.os.Parcelable {
    public static final ** CREATOR;
}

# SdkBox
-keep public class com.sdkbox.** { *; }
-dontwarn com.sdkbox.**

# Android Webkit
-keep public class android.webkit.** { *; }
-dontwarn android.webkit.**

# Google Protobuf
-keep public class com.google.protobuf.** { *; }
-dontwarn com.google.protobuf.**
