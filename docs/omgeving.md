# Ontwikkelomgeving

Waarom deze omgeving: zie [B11](beslissingen.md#b11--ontwikkelen-in-wsl2-ubuntu-2404-met-vs-code)
en [B12](beslissingen.md#b12--build-en-tooling-cmake--ninja-doctest-sanitizers).
Kort: alles in **WSL2 (Ubuntu 24.04)**, bewerken in **VS Code via Remote-WSL**, bouwen
met **CMake + Ninja + g++**.

Laatst bijgewerkt: 29 september 2026

---

## Machine

| Onderdeel | Waarde | Betekenis voor het project |
|---|---|---|
| CPU | AMD Ryzen 7 7840HS (8 cores / 16 threads) | Ruim voor simulatie en voor gevectoriseerde environments op meerdere cores |
| GPU | NVIDIA GeForce RTX 3050 Laptop, 6 GB | Volstaat voor fase 3; kleine policies (MLP/kleine CNN) trainen ook vlot op CPU |
| RAM | 28 GB | Geen beperking |
| OS | Windows 11 + WSL2 met Ubuntu 24.04 | Ubuntu 24.04 is de werkomgeving |

Aanwezig op de Windows-kant (en **niet** gebruikt voor dit project): MinGW-g++ 13.2 via
Strawberry Perl, CMake 3.29, Miniconda (Python 3.13). VS Code staat op Windows en heeft
de extensies C/C++, CMake Tools en Remote-WSL al.

---

## Eenmalige setup

### 1. Toolchain in WSL

In een Ubuntu 24.04-terminal:

```bash
sudo apt update && sudo apt upgrade -y
sudo apt install -y build-essential cmake ninja-build gdb git clang clangd clang-format
```

Controle — verwacht g++ 13.x en CMake ≥ 3.25 (Ubuntu 24.04 levert 3.28):

```bash
g++ --version
cmake --version
ninja --version
```

### 2. Git en GitHub in WSL

WSL heeft een eigen git-configuratie, los van Windows:

```bash
git config --global user.name "Stan Lein"
git config --global user.email "<je GitHub-e-mailadres>"
git config --global init.defaultBranch main
```

Aanmelden bij GitHub, eenvoudigste weg:

```bash
sudo apt install -y gh
gh auth login          # kies GitHub.com, HTTPS, login via browser
```

### 3. Repo klonen — in het Linux-bestandssysteem

```bash
cd ~
git clone https://github.com/stleinp/solo-roguelike.git
cd solo-roguelike
```

**Niet** werken in `/mnt/c/UGENT/...`: bestandstoegang over de Windows-grens maakt
builds en git traag. De oude Windows-checkout in `C:\UGENT\EduMaster\Soloproject\solo-roguelike`
is na de overstap overbodig — werk niet in twee kopieën tegelijk.

Line endings worden door `.gitattributes` (`eol=lf`) geregeld; Windows- en WSL-checkouts
zien dezelfde bestanden.

### 4. VS Code

Vanuit de repomap in WSL:

```bash
code .
```

De eerste keer installeert VS Code automatisch zijn server in WSL. Linksonder staat dan
**WSL: Ubuntu-24.04**. Extensies moeten *in WSL* geïnstalleerd worden (in het
extensiepaneel: "Install in WSL: Ubuntu-24.04"):

- **CMake Tools** — configureren, bouwen, tests draaien vanuit VS Code
- **clangd** (aanbevolen) *of* **C/C++** van Microsoft voor IntelliSense. Niet beide
  tegelijk actief laten: schakel dan IntelliSense van de C/C++-extensie uit. clangd leest
  `compile_commands.json`, dat CMake genereert.
- **C/C++** blijft nuttig voor de debugger (gdb), ook als clangd de IntelliSense doet.

### 5. Claude Code in WSL

```bash
curl -fsSL https://claude.ai/install.sh | bash
cd ~/solo-roguelike
claude
```

Claude Code leest automatisch [`CLAUDE.md`](../CLAUDE.md) in de root; daar staat de
context om meteen verder te kunnen.

---

## Dagelijks gebruik (vanaf M0)

Via `CMakePresets.json`:

```bash
# Debug: met sanitizers, voor ontwikkelen en tests
cmake --preset debug
cmake --build --preset debug
ctest --preset debug

# Release: voor de benchmark (steps/s altijd in Release meten)
cmake --preset release
cmake --build --preset release
./build/release/tools/bench/bench [episodes]
```

`build/` staat in `.gitignore`.

### Grafische frontend (fase 2b, B23)

`delve_gfx` gebruikt raylib 5.5, dat CMake zelf ophaalt (`FetchContent`; de eerste
configure duurt ~45 s). Raylib bouwt GLFW met X11-ondersteuning, dus eenmalig nodig:

```bash
sudo apt install -y libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libxkbcommon-dev
```

Zonder die pakketten: `cmake --preset debug -DDELVE_BUILD_GFX=OFF`; de rest bouwt dan
gewoon. Starten: `./build/debug/src/game/delve_gfx [seed]`.

Getest in WSL2 met WSLg: het venster opent via X11 (`DISPLAY=:0`). Het OpenGL-device is
`llvmpipe` (Mesa, software-rendering), dus zonder GPU-versnelling. Voor 2D-tiles is dat
ruim genoeg; houd het in het oog bij animatie of een eventuele 3D-weergave.

---

## Later: Python en CUDA (fase 3)

Nog **niet** installeren — zie [B15](beslissingen.md#b15--python-pas-vanaf-fase-3).

Wanneer fase 3 begint:
- Eén Python-omgeving voor dit project, ín WSL: Miniforge (conda) of `uv`. Keuze samen
  met de RL-stack (O2). De Windows-Miniconda is vanuit WSL niet bruikbaar.
- De omgeving vastleggen in de repo (`environment.yml` of `pyproject.toml`).
- **CUDA in WSL:** de NVIDIA-driver op *Windows* volstaat. Installeer in WSL géén
  Linux-NVIDIA-driver; alleen PyTorch met CUDA-ondersteuning via pip/conda. Controle:
  `nvidia-smi` in WSL en `torch.cuda.is_available()` in Python.
- Binding C++ → Python via nanobind of pybind11, gebouwd met dezelfde CMake-setup.
