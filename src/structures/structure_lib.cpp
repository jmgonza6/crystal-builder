#include "structure_lib.h"

/*-----------------------------------------------------------------------
 *  Constructor called from menu for a lattice vector only
 *----------------------------------------------------------------------- */

StructureLib::StructureLib(char i_lstyle[], int basisAtoms, double i_alat, double i_clat, std::vector<std::string> elements) : LATTICE()
{   	

    if (strncmp(i_lstyle,"graphene",8)==0) {
        graphene(i_alat,i_clat,basisAtoms);
    } else if (strncmp(i_lstyle,"h-BN",4)==0) {
        h_bn();
    } else if (strncmp(i_lstyle,"MDC MX2-2H",10)==0) {
        mdc_2h(i_alat, i_clat,elements);
    } else if (strncmp(i_lstyle,"MDC MX2-1T",10)==0) {
        mdc_1t(i_alat, i_clat,elements);
    } else if (strncmp(i_lstyle,"TMDC MX2-2H",11)==0) {
        tmdc_2h(i_alat, i_clat,elements);
    } else if (strncmp(i_lstyle,"TMDC MX2-1T",11)==0) {
        tmdc_1t(i_alat, i_clat,elements);
    } else if (strncmp(i_lstyle,"PETN-I",6)==0) {
        petn();
    } else if (strncmp(i_lstyle,"TATB",6)==0) {
        tatb();
    }  else {
        printf("ERROR: That lattice style [ %s ], is unsupported as of now, try again\n",i_lstyle);
    }
}

StructureLib::~StructureLib()
{

}

void
StructureLib::graphene(double i_alat, double i_clat, int basisAtoms)
{
    alat = i_alat;
    blat = i_alat;
    clat = i_clat; 

    species.push_back("C");
    
    if (basisAtoms == 2) {
        // hexagonal
        apcell = basisAtoms;
        ntype  = 1;
        name = memory->new_1d<char>(11);
        basis = new basis_t[apcell];
        strcpy(name, "h-graphene");

        elemCount.push_back(2);

        a1.push_back(1.);
        a1.push_back(0.);
        a1.push_back(0.);

        a2.push_back(0.5);
        a2.push_back(sqrt(0.75));
        a2.push_back(0.);

        a3.push_back(0.);
        a3.push_back(0.);
        a3.push_back(1.);

        basis[0].x = 1./3.;
        basis[0].y = 1./3.;
        basis[0].z = 0.;
        basis[0].typeId = 1;

        basis[1].x = 2./3.;
        basis[1].y = 2./3.;
        basis[1].z = 0.0;
        basis[1].typeId = 1;

    } else if (basisAtoms == 4) {
        // zz along y
        apcell = basisAtoms;
        ntype  = 1;
        name = memory->new_1d<char>(11);
        basis = new basis_t[apcell];
        strcpy(name, "r-graphene");

        elemCount.push_back(4);

        a1.push_back(sqrt(3.));
        a1.push_back(0.);
        a1.push_back(0.);

        a2.push_back(0.);
        a2.push_back(1.);
        a2.push_back(0.);

        a3.push_back(0.);
        a3.push_back(0.);
        a3.push_back(1.);

        basis[0].x = 0.;
        basis[0].y = 0.;
        basis[0].z = 0.;
        basis[0].typeId = 1;

        basis[1].x = 1./6.;
        basis[1].y = 0.5;
        basis[1].z = 0.0;
        basis[1].typeId = 1;

        basis[2].x = 0.5;
        basis[2].y = 0.5;
        basis[2].z = 0.;
        basis[2].typeId = 1;

        basis[3].x = 2./3.;
        basis[3].y = 0.0;
        basis[3].z = 0.0;
        basis[3].typeId = 1;

    } else {
        printf (">>>>>>>> WARNING: Unknown basis atoms, enter 2 or 4\n");
        printf ("                  Basis atoms not set, try again\n");
    }
}

void 
StructureLib::h_bn()
{
    species.push_back("B");
    species.push_back("N");

    double rbn = 1.446;
    alat = blat = sqrt(3.)*rbn;
    clat = 3.35;

    apcell = 2;
    ntype  = 2;
    name = memory->new_1d<char>(5);
    basis = new basis_t[apcell];
    strcpy(name, "h-BN");

    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    a2.push_back(0.5);
    a2.push_back(sqrt(0.75));
    a2.push_back(0.);

    a3.push_back(0.);
    a3.push_back(0.);
    a3.push_back(1.);

    // Boron
    basis[0].x = 1./3.;
    basis[0].y = 1./3.;
    basis[0].z = 0.;
    basis[0].typeId = 1;

    // Nitrogen
    basis[1].x = 2./3.;
    basis[1].y = 2./3.;
    basis[1].z = 0.0;
    basis[1].typeId = 2;

    elemCount.push_back(1);
    elemCount.push_back(1);
}

/* ----------------------------------------------------------------------
   Initialize for BCC lattice
------------------------------------------------------------------------- */
void StructureLib::mdc_2h(double i_alat, double i_clat, std::vector<std::string> elements)
{
    species.push_back(elements[0]);
    species.push_back(elements[1]);
    alat = blat  = i_alat;
    clat =  i_clat;

    char str[MAX_STRING_LENGTH];

    apcell = 3;
    ntype  = 2;
    name = memory->new_1d<char>(MAX_STRING_LENGTH);
    basis = new basis_t[apcell];
    sprintf(str,"%s%s2-2H",elements[0].c_str(),elements[1].c_str());
    strcpy(name, str);

    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    a2.push_back(0.5);
    a2.push_back(sqrt(0.75));
    a2.push_back(0.);

    a3.push_back(0.);
    a3.push_back(0.);
    a3.push_back(1.0);

    //Sn
    basis[0].x = 1.;
    basis[0].y = 0.;
    basis[0].z = 0.;
    basis[0].typeId = 1;

    //S
    basis[1].x = 1./3.;
    basis[1].y = 1./3.;
    basis[1].z = 0.25;
    basis[1].typeId = 2;

    //S
    basis[2].x = 2./3.;
    basis[2].y = 2./3.;
    basis[2].z = 0.75;
    basis[2].typeId = 2;

    elemCount.push_back(1);
    elemCount.push_back(2);
}

/* ----------------------------------------------------------------------
   Initialize for BCC lattice
------------------------------------------------------------------------- */
void StructureLib::mdc_1t(double i_alat, double i_clat, std::vector<std::string> elements)
{
    species.push_back(elements[0]);
    species.push_back(elements[1]);
    alat = blat  = i_alat;
    clat = i_clat;
    char str[MAX_STRING_LENGTH];

    apcell = 3;
    ntype  = 2;
    name = memory->new_1d<char>(MAX_STRING_LENGTH);
    basis = new basis_t[apcell];
    sprintf(str,"%s%s2-1T",elements[0].c_str(),elements[1].c_str());
    strcpy(name, str);

    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    a2.push_back(0.5);
    a2.push_back(sqrt(0.75));
    a2.push_back(0.);

    a3.push_back(0.);
    a3.push_back(0.);
    a3.push_back(1);

    //Sn
    basis[0].x = 1./3.;
    basis[0].y = 1./3.;
    basis[0].z = 0;
    basis[0].typeId = 1;

    //S
    basis[1].x = 2./3.;
    basis[1].y = 2./3.;
    basis[1].z = 0.25;
    basis[1].typeId = 2;

    //S
    basis[2].x = 2./3.;
    basis[2].y = 2./3.;
    basis[2].z = 0.75;
    basis[2].typeId = 2;

    elemCount.push_back(1);
    elemCount.push_back(2);
}

/* ----------------------------------------------------------------------
   Initialize for FCC lattice
------------------------------------------------------------------------- */
void StructureLib::tmdc_2h(double i_alat, double i_clat, std::vector<std::string> elements)
{
    species.push_back(elements[0]);
    species.push_back(elements[1]);
    alat = blat  = i_alat;
    clat = i_clat;

    char str[MAX_STRING_LENGTH];
    
    apcell = 3;
    ntype  = 2;
    name = memory->new_1d<char>(MAX_STRING_LENGTH);
    basis = new basis_t[apcell];
    sprintf(str,"%s%s2-2H",elements[0].c_str(),elements[1].c_str());
    strcpy(name, str);

    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    a2.push_back(0.5);
    a2.push_back(sqrt(0.75));
    a2.push_back(0.);

    a3.push_back(0.);
    a3.push_back(0.);
    a3.push_back(1.);

    // Mo
    basis[0].x = 1./3.;
    basis[0].y = 1./3.;
    basis[0].z = 0.25;
    basis[0].typeId = 1;

    // S
    basis[1].x = 2./3.;
    basis[1].y = 2./3.;
    basis[1].z = 0.121;
    basis[1].typeId = 2;

    // S
    basis[2].x = 2./3.;
    basis[2].y = 2./3.;
    basis[2].z = 0.379;
    basis[2].typeId = 2;

    elemCount.push_back(1);
    elemCount.push_back(2);
}

/* ----------------------------------------------------------------------
   Initialize for FCC lattice
------------------------------------------------------------------------- */
void StructureLib::tmdc_1t(double i_alat, double i_clat, std::vector<std::string> elements)
{
    species.push_back(elements[0]);
    species.push_back(elements[1]);
    alat = blat  = i_alat;
    clat = i_clat;

    char str[MAX_STRING_LENGTH];

    
    apcell = 3;
    ntype  = 2;
    name = memory->new_1d<char>(MAX_STRING_LENGTH);
    basis = new basis_t[apcell];

    sprintf(str,"%s%s2-1T",elements[0].c_str(),elements[1].c_str());
    strcpy(name, str);

    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    a2.push_back(0.5);
    a2.push_back(sqrt(0.75));
    a2.push_back(0.);

    a3.push_back(0.);
    a3.push_back(0.);
    a3.push_back(1.);

    // Mo
    basis[0].x = 1.0;
    basis[0].y =  0.0;
    basis[0].z =  0.;
    basis[0].typeId = 1;

    // S1
    basis[1].x = 1./3.;
    basis[1].y =  1./3.;
    basis[1].z =  0.1;
    basis[1].typeId = 2;

    // S2
    basis[2].x = 2./3.;
    basis[2].y =  2./3.;
    basis[2].z =  -0.1;
    basis[2].typeId = 2;

    // bulk MoS2
    // basis[0].x = 1./3.;
    // basis[0].y = 1./3.;
    // basis[0].z = 0.25;
    // basis[0].typeId = 1;

    // basis[1].x = 2./3.;
    // basis[1].y = 2./3.;
    // basis[1].z = 0.121;
    // basis[1].typeId = 2;

    // basis[2].x = 2./3.;
    // basis[2].y = 2./3.;
    // basis[2].z = 0.379;
    // basis[2].typeId = 2;

    // basis[3].x = 2./3.;
    // basis[3].y = 2./3.;
    // basis[3].z = 0.75;
    // basis[3].typeId = 1;

    // basis[4].x = 1./3.;
    // basis[4].y = 1./3.;
    // basis[4].z = 0.621;
    // basis[4].typeId = 2;

    // basis[5].x = 1./3.;
    // basis[5].y = 1./3.;
    // basis[5].z = 0.879;
    // basis[5].typeId = 2;

    elemCount.push_back(1);
    elemCount.push_back(2);
}


/* ----------------------------------------------------------------------
   Initialize for PETN-I
------------------------------------------------------------------------- */
void StructureLib::petn()
{
    species.push_back("C");
    species.push_back("H");
    species.push_back("N");
    species.push_back("O");
    alat = blat = 9.38;
    clat = 6.70;

    int n1,n2,n3,n4;

    n1 = 10; n2 = 16; n3 = 8; n4 = 24;

    char str[MAX_STRING_LENGTH];

    apcell = n1+n2+n3+n4;
    ntype  = 4;
    name = memory->new_1d<char>(8);
    basis = new basis_t[apcell];
    strcpy(name, "PETN-I");


    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    a2.push_back(0.);
    a2.push_back(1.);
    a2.push_back(0.);

    a3.push_back(0.);
    a3.push_back(0.);
    a3.push_back(1.);
    

    int i;

    // assign atomic types to an integral type
    // used for LAMMPS format
    for (i=0; i<n1; i++) {basis[i].typeId = 1;}
    for (i=n1;i<(n1+n2);i++) {basis[i].typeId = 2;}
    for (i=(n1+n2);i<(n1+n2+n3);i++) {basis[i].typeId = 3;}
    for (i=(n1+n2+n3);i<(n1+n2+n3+n4);i++) {basis[i].typeId = 4;}


    //10 carbons
    basis[0].x = 0.1234250102952819;
    basis[0].y = 0.0609475779157065;
    basis[0].z = 0.1292474135645465;

    basis[1].x = 0.0000088433753593;
    basis[1].y = 0.0000312938510123;
    basis[1].z = -0.0001289387434683;

    basis[2].x = 0.9390663284801790;
    basis[2].y = 0.1234761985360032;
    basis[2].z = 0.8705806396970199;

    basis[3].x = 0.8765258160263396;
    basis[3].y = 0.9390840794302127;
    basis[3].z = 0.1292158064526805;

    basis[4].x = 0.0609647543753718;
    basis[4].y = 0.8765622295947940;
    basis[4].z = 0.8705239110923606;

    basis[5].x = 0.3765353995380687;
    basis[5].y = 0.5609880560834769;
    basis[5].z = 0.3704004486282517;

    basis[6].x = 0.4999903041211348;
    basis[6].y = 0.5000668460980627;
    basis[6].z = 0.4997718346014457;

    basis[7].x = 0.6234774974297089;
    basis[7].y = 0.4390783659028797;
    basis[7].z = 0.3704736564623982;

    basis[8].x = 0.4390251236940225;
    basis[8].y = 0.3766150309860417;
    basis[8].z = 0.6291470750337455;

    basis[9].x = 0.5609529045302394;
    basis[9].y = 0.6234468079461685;
    basis[9].z = 0.6291893809594302;


    //16 hydrogens
    basis[10].x = 0.2166505711833645;
    basis[10].y = 0.0866564632402752;
    basis[10].z = 0.0337873970829672;

    basis[11].x = 0.0887758460757637;
    basis[11].y = 0.1582950926693606;
    basis[11].z = 0.2102739712043039;

    basis[12].x = 0.8417660599147323;
    basis[12].y = 0.0887400617900277;
    basis[12].z = 0.7895813768753572;    

    basis[13].x = 0.9133244844422516;
    basis[13].y = 0.2166442882067104;
    basis[13].z = 0.9660899732148307;

    basis[14].x = 0.7833440948473026;
    basis[14].y = 0.9134255999645062;
    basis[14].z = 0.0336869631183990;

    basis[15].x = 0.9112456237722653;
    basis[15].y = 0.8417278256863617;
    basis[15].z = 0.2101494303404918;

    basis[16].x = 0.1583317521861234;
    basis[16].y = 0.9111470674339939;
    basis[16].z = 0.7894997797847128;

    basis[17].x = 0.0866175890755754;
    basis[17].y = 0.7834053867075369;
    basis[17].z = 0.9660981724840787;

    basis[18].x = 0.3417191249456449;
    basis[18].y = 0.4112951208598201;
    basis[18].z = 0.7103029580539523;

    basis[19].x = 0.4133054984215648;
    basis[19].y = 0.2835143058259915;
    basis[19].z = 0.5335140130472596;

    basis[20].x = 0.2832771135038326;
    basis[20].y = 0.5866293773615049;
    basis[20].z = 0.4658365263556618;    

    basis[21].x = 0.4111949195084212;
    basis[21].y = 0.6583577812463978;
    basis[21].z = 0.2895000318260154;

    basis[22].x = 0.7166258796365181;
    basis[22].y = 0.4133987105547882;
    basis[22].z = 0.4660511557368610;

    basis[23].x = 0.5888023187202147;
    basis[23].y = 0.3416798637707668;
    basis[23].z = 0.2896045487611044;

    basis[24].x = 0.6582972030433654;
    basis[24].y = 0.5887284539207753;
    basis[24].z = 0.7101663992051054;

    basis[25].x = 0.5866920614111345;
    basis[25].y = 0.7166589624675711;
    basis[25].z = 0.5338202257870298;


    //8 nitrogens
    basis[26].x = 0.2947347703961428;
    basis[26].y = 0.9821515438346622;
    basis[26].z = 0.3797160234100750;

    basis[27].x = 0.0178294940200101;
    basis[27].y = 0.2948505339720639;
    basis[27].z = 0.6201136715821307;

    basis[28].x = 0.7051108359660473;
    basis[28].y = 0.0178089851520490;
    basis[28].z = 0.3798864699252137;

    basis[29].x = 0.9821978363821821;
    basis[29].y = 0.7051934528440589;
    basis[29].z = 0.6199950129470895;

    basis[30].x = 0.2051559147175749;
    basis[30].y = 0.4821873037409990;
    basis[30].z = 0.1199689528250137;       

    basis[31].x = 0.7949105514747270;
    basis[31].y = 0.5177725552758387;
    basis[31].z = 0.1199376239742951;    

    basis[32].x = 0.5179199447291160;
    basis[32].y = 0.2050940824513886;
    basis[32].z = 0.8796389829638788;

    basis[33].x = 0.4821452680523884;
    basis[33].y = 0.7947528735792229;
    basis[33].z = 0.8797900233851840;

    //24 oxygens
    basis[34].x = 0.1618852163539675;
    basis[34].y = 0.9510081206349513;
    basis[34].z = 0.2757780650712896;

    basis[35].x = 0.3227915388516672;
    basis[35].y = 0.8914294080478391;
    basis[35].z = 0.5073223144091146;

    basis[36].x = 0.3602029628269364;
    basis[36].y = 0.0901357092857124;
    basis[36].z = 0.3302479620730800;

    basis[37].x = 0.0490072852211034;
    basis[37].y = 0.1620140936696899;
    basis[37].z = 0.7240555554977974;

    basis[38].x = 0.8379628543087850;
    basis[38].y = 0.0489652805247084;
    basis[38].z = 0.2758006459411306;

    basis[39].x = 0.9510664982713117;
    basis[39].y = 0.8380481950095333;
    basis[39].z = 0.7239560410678860;

    basis[40].x = 0.1085867467259578;
    basis[40].y = 0.3229244499835243;
    basis[40].z = 0.4925138229733992;

    basis[41].x = 0.9098154857469101;
    basis[41].y = 0.3602828797252814;
    basis[41].z = 0.6695602499555666;

    basis[42].x = 0.6771102636747253;
    basis[42].y = 0.1085782211312506;
    basis[42].z = 0.5074552651456001;

    basis[43].x = 0.6396155287461972;
    basis[43].y = 0.9098266726469774;
    basis[43].z = 0.3305749330556768;

    basis[44].x = 0.8914621886736926;
    basis[44].y = 0.6771815144293790;
    basis[44].z = 0.4923903682838448;

    basis[45].x = 0.0901625264121581;
    basis[45].y = 0.6396707938975306;
    basis[45].z = 0.6694141140483759;

    basis[46].x = 0.3380730315086735;
    basis[46].y = 0.4511217568637719;
    basis[46].z = 0.2237683802338916;

    basis[47].x = 0.1771333054012358;
    basis[47].y = 0.3914192271522535;
    basis[47].z = 0.9924133032541617;

    basis[48].x = 0.1396348998680058;
    basis[48].y = 0.5901324556449838;
    basis[48].z = 0.1694633332009857;

    basis[49].x = 0.6620266890603006;
    basis[49].y = 0.5489450552701405;
    basis[49].z = 0.2238694482960191;

    basis[50].x = 0.8229479022571921;
    basis[50].y = 0.6084903313233199;
    basis[50].z = 0.9922922798255897;

    basis[51].x = 0.8603795173190565;
    basis[51].y = 0.4098048941058630;
    basis[51].z = 0.1694526029115362;

    basis[52].x = 0.5489743130018822;
    basis[52].y = 0.3380205341087155;
    basis[52].z = 0.7756134800540011;

    basis[53].x = 0.6086810621120478;
    basis[53].y = 0.1771109719520004;
    basis[53].z = 0.0072177208410251;

    basis[54].x = 0.4099954240810333;
    basis[54].y = 0.1395464919568412;
    basis[54].z = 0.8302308898806596;    

    basis[55].x = 0.4509832618773044;
    basis[55].y = 0.6619671914581469;
    basis[55].z = 0.7757170846853854;   

    basis[56].x = 0.3914990872287507;
    basis[56].y = 0.8226488271826732;
    basis[56].z = 0.0076379856097428;

    basis[57].x = 0.5900556737878734;
    basis[57].y = 0.8603147456994008;
    basis[57].z = 0.8302824086065014;

    elemCount.push_back(n1);
    elemCount.push_back(n2);
    elemCount.push_back(n3);
    elemCount.push_back(n4);
}


/* ----------------------------------------------------------------------
   Initialize for PETN-I
------------------------------------------------------------------------- */
void StructureLib::tatb()
{
    species.push_back("C");
    species.push_back("H");
    species.push_back("N");
    species.push_back("O");
    alat = blat = clat = 9.01;

    int n1,n2,n3,n4;

    n1 = 12; n2 = 12; n3 = 12; n4 = 12;

    char str[MAX_STRING_LENGTH];

    apcell = n1+n2+n3+n4;
    ntype  = 4;
    name = memory->new_1d<char>(5);
    basis = new basis_t[apcell];
    strcpy(name, "TATB");


    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    a2.push_back(-0.5);
    a2.push_back(sqrt(3.)*0.5);
    a2.push_back(0.);

    a3.push_back(-0.0240118469);
    a3.push_back(-0.2919282894);
    a3.push_back(0.6970015390);
    

    int i;

    // assign atomic types to an integral type
    // used for LAMMPS format
    for (i=0; i<n1; i++) {basis[i].typeId = 1;}
    for (i=n1;i<(n1+n2);i++) {basis[i].typeId = 2;}
    for (i=(n1+n2);i<(n1+n2+n3);i++) {basis[i].typeId = 3;}
    for (i=(n1+n2+n3);i<(n1+n2+n3+n4);i++) {basis[i].typeId = 4;}



    //12 carbons
    basis[0].x = 0.533225421078354;
    basis[0].y = 0.165145954859975;
    basis[0].z = 0.256847799181831;

    basis[1].x = 0.373304211034811;
    basis[1].y = 0.002585335830150;
    basis[1].z = 0.248726759369562;

    basis[2].x = 0.214961944984982;
    basis[2].y = 0.007316285568772;
    basis[2].z = 0.248726759369562;

    basis[3].x = 0.214427929277325;
    basis[3].y = 0.166670338807011;
    basis[3].z = 0.251115298308347;

    basis[4].x = 0.376025277890588;
    basis[4].y = 0.321767502409863;
    basis[4].z = 0.244268149666836;

    basis[5].x = 0.537898646133822;
    basis[5].y = 0.326821039629845;
    basis[5].z = 0.252070702514404;

    basis[6].x = 0.466774595694231;
    basis[6].y = 0.834854029550889;
    basis[6].z = 0.743152164596877;

    basis[7].x = 0.626695782887236;
    basis[7].y = 0.997414655007152;
    basis[7].z = 0.751273223517474;

    basis[8].x = 0.785038044140087;
    basis[8].y = 0.992683682380962;
    basis[8].z = 0.751273223517474;

    basis[9].x = 0.785572078788985;
    basis[9].y = 0.833329666074985;
    basis[9].z = 0.748884703527783;

    basis[10].x = 0.623974722821893;
    basis[10].y = 0.678232489598502;
    basis[10].z = 0.755731814271107;

    basis[11].x = 0.462101351628306;
    basis[11].y = 0.673178958879267;
    basis[11].z = 0.747929280372632;


    //12 hydrogens
    basis[12].x = 0.467025165613341;
    basis[12].y = -0.142931246616431;
    basis[12].z = 0.238057937215777;    

    basis[13].x = 0.273304211034811;
    basis[13].y = -0.251871478373279;
    basis[13].z = 0.260032517713798;

    basis[14].x = -0.017031243524412;
    basis[14].y = 0.091044063578516;
    basis[14].z = 0.253981548824707;

    basis[15].x = 0.070986996692684;
    basis[15].y = 0.287953862359566;
    basis[15].z = 0.253981548824707;

    basis[16].x = 0.681930308081900;
    basis[16].y = 0.563961747642050;
    basis[16].z = 0.243949662702136;

    basis[17].x = 0.778996742979354;
    basis[17].y = 0.453953817371074;
    basis[17].z = 0.224045152968056;

    basis[18].x = 0.532974862646119;
    basis[18].y = 1.142931253968417;
    basis[18].z = 0.761942026722166;

    basis[19].x = 0.719957288417477;
    basis[19].y = 1.231949896808433;
    basis[19].z = 0.739967446224145;

    basis[20].x = 1.017031219499684;
    basis[20].y = 0.908955931094691;
    basis[20].z = 0.746018434062330;    

    basis[21].x = 0.929012981138629;
    basis[21].y = 0.712046136021683;
    basis[21].z = 0.746018434062330;

    basis[22].x = 0.318069693261142;
    basis[22].y = 0.436038257112083;
    basis[22].z = 0.756050339133994;

    basis[23].x = 0.221003277337608;
    basis[23].y = 0.546046172295165;
    basis[23].z = 0.775954848868074;


    // 12 nitrogens
    basis[24].x = 0.692005882299451;
    basis[24].y = 0.168225920727732;
    basis[24].z = 0.269427448696825;

    basis[25].x = 0.369300808671666;
    basis[25].y = -0.146627249648234;
    basis[25].z = 0.238854116808960;

    basis[26].x = 0.056459867784244;
    basis[26].y = -0.149373393885893;
    basis[26].z = 0.246656688605621;

    basis[27].x = 0.070985482661491;
    basis[27].y = 0.172244042682571;
    basis[27].z = 0.257803203387888;

    basis[28].x = 0.374694065413529;
    basis[28].y = 0.476317110052965;
    basis[28].z = 0.235032443137448;

    basis[29].x = 0.684170743759448;
    basis[29].y = 0.470943956289962;
    basis[29].z = 0.251752234339561;

    basis[30].x = 0.307994116357621;
    basis[30].y = 0.831774040795564;
    basis[30].z = 0.730572515081882;       

    basis[31].x = 0.630699181083965;
    basis[31].y = 1.146627230343737;
    basis[31].z = 0.761145903976264;    

    basis[32].x = 0.943540136748140;
    basis[32].y = 1.149373392395005;
    basis[32].z = 0.753343294281416;

    basis[33].x = 0.929014504299457;
    basis[33].y = 0.827755968572310;
    basis[33].z = 0.742196817397335;

    basis[34].x = 0.625305927716160;
    basis[34].y = 0.523682866813952;
    basis[34].z = 0.764967520641259;

    basis[35].x = 0.315829242757960;
    basis[35].y = 0.529056020449092;
    basis[35].z = 0.748247729439146;

    //12 oxygens
    basis[36].x = 0.833500138561252;
    basis[36].y = 0.306841203868823;
    basis[36].z = 0.282007098371055;

    basis[37].x = 0.692721923306593;
    basis[37].y = 0.029772561755543;
    basis[37].z = 0.264172659400916;

    basis[38].x = 0.051178846281235;
    basis[38].y = -0.291871478373279;
    basis[38].z = 0.237739469200169;

    basis[39].x = -0.081025791661196;
    basis[39].y = -0.149821310399948;
    basis[39].z = 0.252548414091980;

    basis[40].x = 0.238007049519439;
    basis[40].y = 0.478855668304046;
    basis[40].z = 0.230414608981084;

    basis[41].x = 0.513048663412336;
    basis[41].y = 0.617436912135891;
    basis[41].z = 0.235669398276992;

    basis[42].x = 0.166499841327518;
    basis[42].y = 0.693158772923784;
    basis[42].z = 0.717992865566888;

    basis[43].x = 0.307278090788424;
    basis[43].y = 0.970227430380685;
    basis[43].z = 0.735827304537027;

    basis[44].x = 0.948821173076783;
    basis[44].y = 1.291871504897470;
    basis[44].z = 0.762260551744290;

    basis[45].x = 1.081025785381034;
    basis[45].y = 1.149821336924139;
    basis[45].z = 0.747451606852479;

    basis[46].x = 0.761992928968073;
    basis[46].y = 0.521144327788630;
    basis[46].z = 0.769585411963375;

    basis[47].x = 0.486951344458807;
    basis[47].y = 0.382563091447179;
    basis[47].z = 0.764330622508231;

    elemCount.push_back(n1);
    elemCount.push_back(n2);
    elemCount.push_back(n3);
    elemCount.push_back(n4);

}
