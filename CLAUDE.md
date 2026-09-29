# CLAUDE.md

Context voor Claude Code-sessies in deze repo. Houd dit bestand kort en actueel; de
details staan in `docs/`.

## Het project in drie zinnen

**Delve** (werktitel) is een turn-based grid-roguelike op een zelfgeschreven C++-engine,
plus reinforcement-learning-agents die hem leren spelen. De agents zijn geen
tegenstander maar een **meetinstrument**: ze spelen gegenereerde levels honderden keren
om moeilijkheid en balans objectief te bepalen. Het is een persoonlijk leerproject naast
een educatieve master — het doel is leren en technisch scherp blijven, niet omzet.

## De gebruiker

- Stan, industrieel ingenieur informatica, nu in de verkorte educatieve master (UGent).
- Kent C en C++ het best; heeft ervaring met machine learning. Engine-architectuur,
  systeemprogrammeren op deze schaal en RL-in-de-praktijk zijn nieuw.
- Weet bij dit project niet goed waar te beginnen en wil **begeleiding**: leg keuzes en
  het *waarom* uit, werk in kleine stappen, geef een aanbeveling in plaats van een
  opsomming. Het is een leerproject — de gebruiker moet de code begrijpen, niet alleen
  krijgen.
- Werkt ~5–8 uur per week, met onderbrekingen van soms twee weken.
- Communicatie en documentatie in het **Nederlands**.

## Status

- **Fase 0 (planning), fase 1 (M0 t/m M5), fase 2 (terminal, B22) en fase 2b (2D-grafisch
  met raylib, G0–G3, B23; playtests geslaagd): afgerond.**
  Toolchain aanwezig; `cmake --preset debug && cmake --build --preset debug && ctest --preset debug`.
- **Volgende stap: fase 3** (RL-omgeving), te beginnen met O2 (RL-stack). Zie
  `docs/projectplan.md` (sectie "Fase 3"). 3D-weergave is uitgesteld maar niet uitgesloten
  (B24); niet oppakken zonder dat de gebruiker erom vraagt.
- Laatste stand altijd in `docs/devlog.md` (nieuwste bovenaan).

## Omgeving

WSL2 Ubuntu 24.04, repo in `~/solo-roguelike`, VS Code via Remote-WSL.
C++20, CMake + Ninja, g++, doctest via FetchContent, ASan/UBSan in Debug.
Geen Python tot fase 3. Details: `docs/omgeving.md`.

## Regels die de code moet respecteren

1. **Strikte scheiding simulatie / rendering / input** (B5). `src/sim/` is een eigen
   CMake-library die tegen niets linkt en geen I/O doet: geen `<iostream>`, geen
   bestanden, geen klok. Game, bench en tests linken tegen `sim`, nooit omgekeerd.
2. **Determinisme** (B6, B14): eigen RNG (geen `<random>`-distributies), RNG-state in de
   `State`, geen iteratie over `unordered_*` waar volgorde telt, geen floats in
   spelregels, geen globale state, geen pointers als sleutel, vaste verwerkingsvolgorde.
   De determinismetest met golden hash verandert alleen bewust.
3. **Data-georiënteerd** (B13): platte structs, `std::array`/`std::vector`,
   `enum class`, vrije functies zoals `StepResult step(State&, Action)`, indices in
   plaats van pointers. Geen `virtual`-hiërarchieën, geen ECS-framework.
4. **Deze game's engine, geen algemeen framework.** Woorden als "plugin", "herbruikbaar"
   of "asset pipeline" zonder concrete nood = stoppen.
5. **Throughput meten vanaf M5**, altijd in een Release-build.
6. **raylib alleen in `delve_gfx`** (B23): nooit gelinkt door `sim`, `bench`, `tests` of
   de tekstrenderer. Floats en tijd mogen in de weergavelaag (animatie), nooit in `sim`.
   Simulatie blijft 2D; 3D alleen als weergave en niet vóór fase 3 (B24 beschrijft de overgang).

## Werkafspraken

- Eén mijlpaal per keer; elke mijlpaal eindigt met iets dat compileert en getest is.
- Verander één of twee dingen tegelijk.
- Geen scope uitbreiden (items, fog of war, generatie, Python) vóór de fase daarvoor.
- Bij elke beslissing: `docs/beslissingen.md` bijwerken, mét de reden.
- Na elke sessie: voorstel voor een devlog-entry in `docs/devlog.md`.
- Bij statuswijziging: dit bestand en de statusregel in `docs/README.md` bijwerken.
- Commit/push alleen op vraag van de gebruiker.

## Documentatie

| Bestand | Inhoud |
|---|---|
| `docs/README.md` | Projectoverzicht |
| `docs/projectplan.md` | Fasen 1–6 (incl. 2b), mijlpalen M0–M5 en G0–G3, repostructuur, waarschuwingssignalen |
| `docs/beslissingen.md` | Beslissingen B1–B24 en open vragen O2, O4–O7 |
| `docs/omgeving.md` | WSL-setup, toolchain, VS Code, later Python/CUDA |
| `docs/devlog.md` | Wat er per sessie gebeurd is |
| `docs/brainstorm.md` | Achtergrond; verworpen richtingen (geen sturing voor de code) |
| `docs/referenties.md` | Leeslijst per fase |

## Open punten

- Vijf acties met bump-to-attack (B18).
- Code en commentaar zijn Engels, documentatie Nederlands (B17).
- Nog open volgens planning: O2 RL-stack (begin fase 3), O4 observatievorm en O5 reward-ontwerp (fase 3), O6 publiek/privé, O7 lesmateriaal.
- Klein: formulering B2 ("real-time loops", "frame budget") past niet bij de turn-based
  scope; bron UGent@Work in `brainstorm.md` verwijst naar een partijwebsite.
