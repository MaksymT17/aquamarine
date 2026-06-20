# Aquamarine

**High-Performance Image Analysis and Change Detection Engine**

Aquamarine is a highly optimized, precision-engineered tool designed for advanced image recognition, detection, and analysis. Its core capability lies in identifying delta-changes (differences) between images, making it exceptionally suited for movement detection, tracking object appearances and disappearances, and filtering out environmental noise such as lighting changes or micro-movements (e.g., wind affecting foliage).

## Architecture & Objectives

The primary objective of Aquamarine is to provide an optimal, computationally efficient solution for recognizing image characteristics within strictly defined hardware constraints. 

- **Configurable Precision:** Algorithmic parameters are strictly controlled via a centralized configuration file. These parameters define the calculation depth and precision, ensuring the software adapts to both high-end computational platforms and resource-constrained hardware.
- **Noise Rejection:** The engine is calibrated to isolate significant positional changes (such as human movement) while aggressively filtering out irrelevant environmental artifacts.

### Analytical Output

The engine outputs analytical bounding regions—rectangular coordinates identifying areas where significant color or pixel-cluster shifts have occurred. This allows downstream systems to process only the relevant telemetry rather than scanning the entire image payload.

*Note: Formal compliance and stress testing stages are currently in progress.*

---

## Standalone GUI Interface (Qt)

Aquamarine includes a standalone, cross-platform graphical user interface powered by Qt. This GUI provides real-time telemetry and visualization of the detection algorithm, allowing engineers to interactively adjust detection parameters (e.g., affinity thresholds, calculation timeouts) and instantly view the resultant bounding regions overlaying the base imagery.

### Qt Installation Guidelines

To build the standalone Qt application, **Qt6** is required. The build system will automatically detect the presence of Qt6 and compile the graphical interface alongside the core library. If Qt6 is absent, the build system gracefully falls back to compiling only the headless backend service.

**macOS:**
```bash
brew install qt
# Note: Ensure Qt is linked or its path is accessible by CMake.
```

**Linux (Debian/Ubuntu):**
```bash
sudo apt update
sudo apt install qt6-base-dev qt6-base-dev-tools libqt6svg6-dev
```

**Windows:**
1. Download the [Qt Online Installer](https://www.qt.io/download-qt-installer).
2. During installation, select **Qt 6.x** for your target compiler (e.g., MSVC or MinGW).
3. Ensure the Qt bin directory is added to your system `PATH`, or set `CMAKE_PREFIX_PATH` to your Qt installation directory during the CMake configuration step.

---

## Build Instructions

For detailed dependency requirements and build logic, please refer to the [Build Documentation](https://github.com/MaksymT17/aquamarine/wiki/Details-of-build-and-implementation).

You can build all targets natively using the provided build script:
```bash
./build.sh
```

---

## Licensing & Restrictions

**Compliance and Usage Policies:**

- **Commercial Usage:** Commercial integration of this source code is strictly prohibited without explicit, written authorization from the original author.
- **Privacy Compliance:** Any deployment of this software that violates local or international privacy laws is strictly forbidden.
- **Defense & Military Applications:** For defense-specific or military purposes, the use of this software is exclusively restricted to Ukraine, the European Union, and the United States of America, and requires prior author agreement.

*For inquiries, licensing, or suggestions, please contact the author directly via private message or email.*
