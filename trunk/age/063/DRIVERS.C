#include "drivers.h"


// 
// A list of all of the drivers available to age
//

extern struct GAMEINFO Centipede;
extern struct GAMEINFO Millipede;
extern struct GAMEINFO Scramble;
extern struct GAMEINFO Atlantis;
extern struct GAMEINFO TheEnd;
extern struct GAMEINFO RallyX;
extern struct GAMEINFO NewRallyX;
extern struct GAMEINFO Galaxian;
extern struct GAMEINFO Pisces;
extern struct GAMEINFO WarOfBugs;
extern struct GAMEINFO Vanguard;
extern struct GAMEINFO Frogger;
extern struct GAMEINFO MrDo;
extern struct GAMEINFO DigDug2;
extern struct GAMEINFO Mappy;
extern struct GAMEINFO Druaga;
extern struct GAMEINFO Motos;
extern struct GAMEINFO DKong;
extern struct GAMEINFO DKongJnr;
extern struct GAMEINFO BRubber;
extern struct GAMEINFO MrDot;
extern struct GAMEINFO MrLo;
extern struct GAMEINFO Ladybug;
extern struct GAMEINFO DoCastle;
extern struct GAMEINFO Amidar;
extern struct GAMEINFO AmidarJp;
extern struct GAMEINFO Turtles;
extern struct GAMEINFO Turpin;
extern struct GAMEINFO Pooyan;
extern struct GAMEINFO MrDot;
extern struct GAMEINFO MrLo;
extern struct GAMEINFO Ladybug;
extern struct GAMEINFO DoCastle;
extern struct GAMEINFO Amidar;
extern struct GAMEINFO AmidarJp;
extern struct GAMEINFO Turtles;
extern struct GAMEINFO Turpin;
extern struct GAMEINFO Pooyan;

// jerry's additions:
extern struct GAMEINFO PacMan;
extern struct GAMEINFO MsPacMan;
extern struct GAMEINFO CrushRoller;
extern struct GAMEINFO Pengo;
extern struct GAMEINFO Tron;

extern struct GAMEINFO MsPacAtk;
extern struct GAMEINFO Piranha;
extern struct GAMEINFO PuckMan;
extern struct GAMEINFO Hangly;
extern struct GAMEINFO PacPlus;
extern struct GAMEINFO PacMod;
extern struct GAMEINFO NamcoPac;
extern struct GAMEINFO PacManJP;
extern struct GAMEINFO Pengoa;
extern struct GAMEINFO Penta;
extern struct GAMEINFO PacManBL; // let's not forget this one!

extern struct GAMEINFO Reactor;
extern struct GAMEINFO Qbert;
extern struct GAMEINFO QbertJP;
extern struct GAMEINFO QbertQub;
extern struct GAMEINFO Mplanets;
extern struct GAMEINFO Krull;
extern struct GAMEINFO Stooges;

extern struct GAMEINFO Galaga;
extern struct GAMEINFO GalagaNM;
extern struct GAMEINFO GalagaBL;
extern struct GAMEINFO Gallag;

extern struct GAMEINFO Galmidw;
extern struct GAMEINFO Galnamco;
extern struct GAMEINFO SuperG;
extern struct GAMEINFO GalaPX;
extern struct GAMEINFO GalaP1;
extern struct GAMEINFO GalaP4;
extern struct GAMEINFO GalTurbo;
extern struct GAMEINFO JapIrem;
extern struct GAMEINFO Uniwars;
extern struct GAMEINFO RedUfo;

extern struct GAMEINFO JRPacMan;
extern struct GAMEINFO DigDugNM;
extern struct GAMEINFO DigDugAT;

// chris' additions:
extern struct GAMEINFO c1942;
extern struct GAMEINFO BoblBobl;
extern struct GAMEINFO Bombjack;



// list of all of the drivers
struct GAMEINFO * Drivers[] = {
   &Centipede,
   &Millipede,
   &PacMan,
   &MsPacMan,
   &Pengo,
   &CrushRoller,
   &Scramble,
   &Atlantis,
   &TheEnd,
   &RallyX,
   &NewRallyX,
   &Galaxian,
   &Pisces,
   &WarOfBugs,
   &PacManBL,
   &Vanguard,
   &Frogger,
   &BoblBobl,
   &c1942,
   &Bombjack,
   &MrDo,
   &Tron,
   &DigDug2,
   &Mappy,
   &Motos,
   &Druaga,
   &MsPacAtk,
   &Piranha,
   &PuckMan,
   &Hangly,
   &PacPlus,
   &PacMod,
   &NamcoPac,
   &PacManJP,
   &Pengoa,
   &Penta,
   &DKong,
   &DKongJnr,
	&BRubber,
   &Reactor,
   &Qbert,
   &QbertJP,
   &QbertQub,
   &Mplanets,
   &Krull,
   &Stooges,
	&DoCastle,
	&Amidar,
	&AmidarJp,
	&Turtles,
	&Turpin,
	&Pooyan,
	&MrDot,
	&MrLo,
	&Ladybug,
    &Galaga,
    &GalagaNM,
    &GalagaBL,
    &Gallag,
    &Galmidw,
    &Galnamco,
    &SuperG,
    &GalaPX,
    &GalaP1,
    &GalaP4,
    &GalTurbo,
    //&JapIrem,
    //&Uniwars,
    &RedUfo,
    &JRPacMan,
    &DigDugNM,
    &DigDugAT,
   0  // list terminator - must be last
};
