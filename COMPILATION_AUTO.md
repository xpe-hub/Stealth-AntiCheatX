# Stealth-AntiCheatX - Compilación Automatizada

## Compilación Automática con GitHub Actions

Este proyecto está configurado para compilación automática usando GitHub Actions con MinGW-w64.

### ✅ Funcionalidad Automática

- **Compilación Cross-Platform**: GitHub Actions compila automáticamente en Windows
- **Artifacts**: El EXE se sube automáticamente como artifact para descarga
- **Releases**: En cada release, el EXE se sube automáticamente
- **MinGW Compilation**: Compilación optimizada para máximo rendimiento

### 🔄 Proceso Automático

1. **Push a main/master**: GitHub Actions compila automáticamente
2. **Create Release**: El EXE se sube automáticamente a la release
3. **Download**: Descarga el EXE desde Actions o Release

### 📥 Descarga del EXE

**Opción 1 - Desde Actions:**
1. Ve a: https://github.com/xpe-hub/Stealth-AntiCheatX/actions
2. Selecciona el workflow más reciente
3. Download "Stealth-AntiCheatX" artifact

**Opción 2 - Desde Release:**
1. Ve a: https://github.com/xpe-hub/Stealth-AntiCheatX/releases
2. Descarga el EXE desde la última release

### 🛠️ Compilación Manual (Local)

**Con MinGW (Windows):**
```bash
build_mingw.bat
```

**Con Visual Studio:**
```bash
build_release.bat
```

### ⚙️ Configuración Requerida

El GitHub Token ya está configurado en el repositorio para GitHub Actions.

### 🚀 Próximos Pasos

1. Push a main → GitHub Actions compila automáticamente
2. Crear release → EXE subido automáticamente
3. Descargar y probar el EXE

---

**Estado actual**: Listo para compilación automática ✅