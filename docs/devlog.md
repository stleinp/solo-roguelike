# Devlog

Na elke werksessie een korte notitie: wat gedaan, wat geleerd, welke mijlpaal af, en —
vanaf M5 — het gemeten aantal steps/seconde. Nieuwste bovenaan. Doel: na twee weken
stilstand zonder aanloop verder kunnen, en expliciteren wat je geleerd hebt.

---

## 29 september 2026 (sessie 2, vervolg) — M3 RNG en vijand af

**Gedaan**
- `src/sim/rng.hpp/.cpp`: splitmix64, xoshiro256**, `range(n)` zonder bias. Seed via
  `parse_map(text, seed)`. Beslissingen in B20.
- Vijand achtervolgt of dwaalt (1 op 4), slaat als hij naast de speler staat.
- 11 nieuwe tests (29 in totaal): RNG (referentiewaarde splitmix64, uniformiteit,
  replay van een gekopieerde state) en vijand (invarianten over 50 seeds, corridor,
  zelfde seed = zelfde spel, andere seed = ander pad). Groen met ASan/UBSan.

**Geleerd / onthouden**
- Rejection sampling: de eerste `2^64 mod n` waarden weggooien geeft een exact uniforme
  `range`; `(0 - n) % n` in unsigned rekenen is die drempel.
- De vijand kan achter een muur blijven vastzitten (geen pathfinding). Bewust; kijk er
  opnieuw naar als de agent er last van krijgt.

**Volgende:** M4 — hash van de eindtoestand en golden-waarde.

---

## 29 september 2026 (sessie 2, vervolg) — M2 `step()` af

**Gedaan**
- `src/sim/step.hpp/.cpp`: `StepResult step(State&, Action)`, `Outcome`, `outcome_of`.
  Regels vastgelegd in B19.
- 13 nieuwe tests, één of twee per regel (18 in totaal); alles groen met ASan/UBSan.
- De vijand slaat al als hij naast de speler staat, zodat `Lost` in M2 testbaar is;
  bewegen en RNG volgen in M3.

**Geleerd / onthouden**
- Uitkomst afleiden uit de state ipv een vlag bijhouden houdt `State` de enige bron van
  waarheid, wat later ook de determinismehash (M4) eenvoudiger maakt.

**Volgende:** M3 — eigen RNG en vijandgedrag.

---

## 29 september 2026 (sessie 2, vervolg) — M1 datamodel af

**Gedaan**
- `src/sim/types.hpp` (`Action`, `Tile`, `Pos`), `state.hpp/.cpp` (`State`,
  `parse_map`). Kaartformaat: `#` muur, `.` vloer, `>` doel, `@` speler, `E` vijand.
- `parse_map` valideert: rechthoekig, precies één speler en vijand, minstens één doel.
  Geeft `std::optional<State>`.
- 5 tests op inlezen en grenzen; alles groen met ASan/UBSan.
- B18: vijf acties met bump-to-attack.

**Geleerd / onthouden**
- `rng_state` staat al in `State` als lege array; de echte RNG komt in M3.
- Foutmeldingen bij ongeldige kaarten zijn bewust weggelaten (`nullopt`); pas toevoegen
  als het debuggen erom vraagt.

**Volgende:** M2 — `step()`.

---

## 29 september 2026 (sessie 2) — M0 skelet af

**Gedaan**
- Toolchain gecontroleerd; `ninja-build` en `clang-format` bijgeïnstalleerd.
- Skelet: `CMakeLists.txt`, `CMakePresets.json` (debug/release), `src/sim/` (lege
  library met placeholder `sim::version()`), `tests/` met doctest via FetchContent,
  `.gitignore`, `.clang-format`. Warnings aan; ASan/UBSan alleen in Debug, via een
  `delve_options` interface-library zodat doctest niet aan onze warnings hoeft te voldoen.
- Debug + Release bouwen; `ctest` slaagt (1/1).
- B17 vastgelegd: code Engels, docs Nederlands. Paden in docs rechtgezet
  (`~/solo-roguelike`).

**Geleerd**
- `sim` linkt niets en de opties zijn `PRIVATE`: consumenten erven geen sanitizers
  via `sim`, maar linken ze zelf via `delve_options`. Beide kanten moeten dezelfde
  sanitizers hebben, anders klopt het niet.

**Volgende:** M1 — datamodel; beslis 5 of 6 acties.

---

## 29 september 2026 — Planning afgerond, taal en omgeving gekozen

**Gedaan**
- Brainstorm uitgewerkt tot projectplan, beslissingslogboek, brainstormverslag en
  leeslijst.
- Documentatie verplaatst naar `docs/`; links in de README rechtgezet.
- Taal gekozen: **C++20** ([B10](beslissingen.md#b10--c20-als-implementatietaal-was-o1)).
  C en C++ zijn het best gekend; nieuwe taal + nieuw domein + nieuwe toolchain zou te
  veel tegelijk zijn.
- Omgeving gekozen: **WSL2 Ubuntu 24.04 + VS Code** (B11), met **CMake + Ninja,
  doctest, sanitizers** (B12). Doorslaggevend: de Python-binding in fase 3 en
  RL-tooling werken op Linux zonder gedoe; op Windows botst MinGW met de MSVC-Python.
- Codestijl (B13), determinismeregels (B14), "Python pas vanaf fase 3" (B15) en de
  MVP-definitie (B16) vastgelegd.
- Fase 1 opgedeeld in mijlpalen M0–M5 (zie projectplan).
- `CLAUDE.md` en `docs/omgeving.md` toegevoegd zodat een nieuwe sessie in WSL meteen
  mee is.

**Geleerd / onthouden**
- `std::uniform_int_distribution` is niet portabel deterministisch — dezelfde seed kan
  per standaardbibliotheek andere getallen geven. Eigen RNG schrijven.
- Een Python-extensie bouwen met MinGW tegen een MSVC-gebouwde Python geeft
  ABI-problemen. Nog een reden voor Linux.
- De scheiding sim/render (B5) kun je laten afdwingen door de build: `sim` als eigen
  CMake-library die tegen niets linkt.

**Losse eindjes** (zie ook `CLAUDE.md`)
- Aantal acties vastleggen bij M1: vijf (bump-to-attack) of zes (expliciete aanval).
- B2 noemt "real-time loops" en "performance onder een frame budget" als leerwinst,
  terwijl real-time buiten scope valt; de echte performance-uitdaging hier is
  simulatie-throughput. Formulering eventueel aanpassen.
- In `brainstorm.md` verwijst de bron voor het UGent@Work-planlastonderzoek naar een
  partijwebsite in plaats van naar het onderzoek zelf. Alleen relevant als het
  edtech-idee ooit terugkomt.

**Volgende sessie:** repo klonen in WSL, toolchain installeren (`omgeving.md`), dan M0.
