# Delve — roguelike-engine met lerende agents

> **Werktitel.** "Delve" is een plaatshouder; hernoem gerust.

Een turn-based roguelike op een zelfgeschreven engine, plus reinforcement-learning-agents
die hem leren spelen. Die agents worden niet gebruikt als tegenstander, maar als
**meetinstrument**: om moeilijkheidsgraad en balans van procedureel gegenereerde levels
objectief te bepalen.

## Waarom dit project bestaat

Dit is een persoonlijk leerproject naast de verkorte educatieve master. Het doel is
**niet** omzet. Het doel is technisch scherp blijven in een jaar waarin de opleiding
inhoudelijk elders ligt, en daarbij bewust dingen leren die buiten de comfortzone van
een gewone applicatiestack vallen.

Commerciële kansen zijn meegenomen als ze zich voordoen, maar er wordt **niet** naar
ontworpen. Zie [`brainstorm.md`](brainstorm.md) voor hoe die keuze tot stand kwam.

## De kernidee in één alinea

Een eigen engine is hier geen luxe maar een vereiste: reinforcement learning heeft
deterministische simulatie, een headless modus en hoge throughput nodig, en dat zijn
precies de dingen die Unity of Godot je niet geven. Dezelfde eigenschappen leveren
gratis replays, save/load en deterministische tests op. Engine en RL zijn dus niet twee
losse projecten — het één volgt uit het ander.

## Wat het wordt

- Een deterministische, headless simuleerbare roguelike-kern (grid, turn-based)
- Een speelbare game daarbovenop, met eigen renderer en input
- Een trainbare environment-binding voor RL
- Een agent die levels betrouwbaar uitspeelt
- Procedurele generatie, met de agent als automatische playtester

## Wat het (voorlopig) niet wordt

- Geen algemene, herbruikbare engine. Dit is *deze game's* engine.
- Geen real-time actie, geen 3D, geen fog of war in de eerste versies.
- Geen RL-getrainde vijanden als fundament. Zie [beslissing B8](beslissingen.md#b8--agents-zijn-meetinstrument-geen-tegenstander).
- Geen commercieel product.

## Status

**Fase 1 — M0 t/m M3 afgerond.** `sim` bevat `State`, `parse_map`, `step()`, eigen RNG
en een vijand; tests slagen in Debug (ASan/UBSan). Volgende: M4 (determinismetest).
RL-stack beslist pas in fase 3.

## Documentatie

| Document | Inhoud |
|---|---|
| [`projectplan.md`](projectplan.md) | Gefaseerd plan, mijlpalen (incl. M0–M5 van fase 1), afbakening, risico's |
| [`beslissingen.md`](beslissingen.md) | Beslissingslogboek: wat vastligt, wat open is, en waarom |
| [`omgeving.md`](omgeving.md) | Ontwikkelomgeving: WSL2, toolchain, VS Code, Python later |
| [`devlog.md`](devlog.md) | Devlog: wat er per sessie gedaan en geleerd is |
| [`brainstorm.md`](brainstorm.md) | Achtergrond: welke richtingen overwogen zijn en waarom ze afvielen |
| [`referenties.md`](referenties.md) | Leeslijst en tooling |
| [`../CLAUDE.md`](../CLAUDE.md) | Context voor Claude Code-sessies |

## De enige architectuurbeslissing die nu al goed moet

**Simulatie, rendering en input blijven strikt gescheiden vanaf de eerste regel code.**
Als rendering in de game logic verweven raakt, kan er in fase 3 niet meer headless
getraind worden zonder alles te herschrijven. Alle andere keuzes zijn later nog om te
gooien; deze niet.

## Volgende stap

Fase 1, mijlpaal **M4 — determinismetest**. Zie
[`projectplan.md`](projectplan.md#fase-1--deterministische-headless-kern).

## Werkregels

- Verander één of twee dingen tegelijk, niet vijf. Nieuwe taal *en* nieuw domein is
  prima; nieuwe taal, nieuw domein, nieuw paradigma en nieuwe toolchain niet.
- Elke fase moet op zichzelf af zijn. Stoppen na fase 2 moet iets opleveren waar je
  trots op bent.
- Korte mijlpalen. Dit project ligt regelmatig twee weken stil; het moet daarna zonder
  aanloop op te pikken zijn.
- Houd een devlog bij. Niet voor publiek — het dwingt tot expliciteren wat je geleerd
  hebt, en dat is toevallig precies de vaardigheid waar de opleiding om draait.
- Meet steps/seconde vanaf dag één. Throughput is de belangrijkste variabele van dit
  project.
