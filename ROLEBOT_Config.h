#ifndef ROLEBOT_Config_H
#define ROLEBOT_Config_H

/**
 * @file ROLEBOT_Config.h
 * @brief ROLEBOT modüllerinin etkinleştirilmesi/devre dışı bırakılması için yapılandırma dosyası.
 * @brief Configuration file for enabling/disabling ROLEBOT modules.
 *
 * Kullanılacak modülleri etkinleştirmek için `#define` satırını açın.
 * Devre dışı bırakılan modüller derlemeden hariç tutulur, hafıza kullanımı optimize edilir.
 * Activate desired modules by uncommenting `#define` lines.
 * Disabled modules are excluded from compilation, optimizing memory usage.
 *
 * ------------------------------
 * Avantajlar / Benefits:
 * ------------------------------
 * ✅ Bellek kullanımını azaltır. / Reduces memory usage.
 * ✅ Modüler ve yönetimi kolaydır. / Modular and easy to manage.
 * ✅ Derleme süresini optimize eder. / Optimized compilation time.
 */

// Modülleri etkinleştir/devre dışı bırak
// Enable/disable modules
// Etkinleştirmek için yorumu kaldırın / Uncomment to activate

 #define USE_SERVER // Web Sunucusu desteğini etkinleştir / Enable Web Server support
 #define USE_FIREBASE     // Firebase desteğini etkinleştir / Enable Firebase support
 #define USE_WIFI        // WiFi desteğini etkinleştir / Enable WiFi support

#endif // ROLEBOT_Config_H
