#include "structure_lib.h"

/*-----------------------------------------------------------------------
 *  Constructor called from menu for a lattice vector only
 *----------------------------------------------------------------------- */

StructureLib::StructureLib(char i_lstyle[], int basisAtoms, double i_alat, double i_cta, std::vector<std::string> elements) : LATTICE()
{   	
    cta = i_cta;
    if (strncmp(i_lstyle,"graphene",8)==0) {
        graphene(i_alat,basisAtoms);
    } else if (strncmp(i_lstyle,"h-BN",4)==0) {
        h_bn();
    } else if (strncmp(i_lstyle,"MDC MX2-2H",10)==0) {
        mdc_2h(i_alat, elements);
    } else if (strncmp(i_lstyle,"MDC MX2-1T",10)==0) {
        mdc_1t(i_alat, elements);
    } else if (strncmp(i_lstyle,"TMDC MX2-2H",11)==0) {
        tmdc_2h(i_alat, elements);
    } else if (strncmp(i_lstyle,"TMDC MX2-1T",11)==0) {
        tmdc_1t(i_alat, elements);
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
StructureLib::graphene(double i_alat, int basisAtoms)
{
    alat = i_alat;
    blat = i_alat;
    clat = 3.35; // Å

    species.push_back("C");
    
    // hexagonal
    if (basisAtoms == 2) {
        memory->new_1d(name,9);
        strcpy(name, "h-graphene");
        apcell = basisAtoms;
        ntype  = 1;
        memory->new_1d(nt,ntype);
        // nt[0] = 2;
        elemCount.push_back(2);

        a1.push_back(1.);
        a1.push_back(0.);
        a1.push_back(0.);

        a2.push_back(0.5);
        a2.push_back(sqrt(0.75));
        a2.push_back(0.);

        a3.push_back(0.);
        a3.push_back(0.);
        a3.push_back(cta);

        memory->new_2d(basis,apcell,3);
        memory->new_1d(atom_type,apcell);

        for (int i=0; i<apcell; i++) atom_type[i] = 1;
        basis[0][0] = 1./3.;
        basis[0][1] = 1./3.;
        basis[0][2] = 0.;

        basis[1][0] = 2./3.;
        basis[1][1] = 2./3.;
        basis[1][2] = 0.;
    } else if (basisAtoms == 4) {
        memory->new_1d(name,9);
        strcpy(name, "r-graphene");
        // zz along y
        apcell = basisAtoms;
        ntype  = 1;
        memory->new_1d(nt,ntype);
        // nt[0] = 4;
        elemCount.push_back(4);

        a1.push_back(sqrt(3.));
        a1.push_back(0.);
        a1.push_back(0.);

        a2.push_back(0.);
        a2.push_back(1.);
        a2.push_back(0.);

        a3.push_back(0.);
        a3.push_back(0.);
        a3.push_back(cta);

        memory->new_2d(basis,apcell,3);
        memory->new_1d(atom_type,apcell);

        for (int i=0; i<apcell; i++) atom_type[i] = 1;
        basis[0][0] = 0.;
        basis[0][1] = 0.;
        basis[0][2] = 0.;

        basis[1][0] = 1./6.;
        basis[1][1] = 0.5;
        basis[1][2] = 0.;

        basis[2][0] = 0.5;
        basis[2][1] = 0.5;
        basis[2][2] = 0.;

        basis[3][0] = 2./3.;
        basis[3][1] = 0;
        basis[3][2] = 0.;
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

    memory->new_1d(name,9);
    strcpy(name, "h-BN");
    apcell = 2;
    ntype  = 2;

    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    a2.push_back(0.5);
    a2.push_back(sqrt(0.75));
    a2.push_back(0.);

    a3.push_back(0.);
    a3.push_back(0.);
    a3.push_back(clat);

    memory->new_2d(basis,apcell,3);
    memory->new_1d(atom_type,apcell);


    atom_type[0] = 1;
    basis[0][0] = 1./3.;
    basis[0][1] = 1./3.;
    basis[0][2] = 0.;

    atom_type[1] = 2;
    basis[1][0] = 2./3.;
    basis[1][1] = 2./3.;
    basis[1][2] = 0.;

    memory->new_1d(nt,ntype);
    // nt[0] = 1;
    // nt[1] = 1;

    elemCount.push_back(1);
    elemCount.push_back(1);
}

/* ----------------------------------------------------------------------
   Initialize for BCC lattice
------------------------------------------------------------------------- */
void StructureLib::mdc_2h(double i_alat, std::vector<std::string> elements)
{
    species.push_back(elements[0]);
    species.push_back(elements[1]);
    alat = blat = clat =  i_alat;
    char str[MAX_STRING_LENGTH];
    
    memory->new_1d(name,9);
    sprintf(str,"%s%s2-2H",elements[0].c_str(),elements[1].c_str());
    strcpy(name, str);

    apcell = 3;
    ntype  = 2;

    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    a2.push_back(0.5);
    a2.push_back(sqrt(0.75));
    a2.push_back(0.);

    a3.push_back(0.);
    a3.push_back(0.);
    a3.push_back(4.6);

    memory->new_2d(basis,apcell,3);
    memory->new_1d(atom_type,apcell);

    atom_type[0] = 1;

    basis[0][0] = 1.0;
    basis[0][1] = 0.0;
    basis[0][2] = 0.;

    atom_type[1] = 2;
    basis[1][0] = 1./3.;
    basis[1][1] = 1./3.;
    basis[1][2] = 0.1;

    atom_type[2] = 2;
    basis[2][0] = 2./3.;
    basis[2][1] = 2./3.;
    basis[2][2] = -0.1;

    memory->new_1d(nt,ntype);
    // nt[0] = 1;
    // nt[1] = 2;
    elemCount.push_back(1);
    elemCount.push_back(2);
}

/* ----------------------------------------------------------------------
   Initialize for BCC lattice
------------------------------------------------------------------------- */
void StructureLib::mdc_1t(double i_alat, std::vector<std::string> elements)
{
    species.push_back(elements[0]);
    species.push_back(elements[1]);
    alat = blat = clat =  i_alat;
    char str[MAX_STRING_LENGTH];
    
    memory->new_1d(name,9);
    sprintf(str,"%s%s2-1T",elements[0].c_str(),elements[1].c_str());
    strcpy(name, str);

    apcell = 3;
    ntype  = 2;

    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    a2.push_back(0.5);
    a2.push_back(sqrt(0.75));
    a2.push_back(0.);

    a3.push_back(0.);
    a3.push_back(0.);
    a3.push_back(4.6);

    memory->new_2d(basis,apcell,3);
    memory->new_1d(atom_type,apcell);


    atom_type[0] = 1;
    basis[0][0] = 1./3.;
    basis[0][1] = 1./3.;
    basis[0][2] = 0.;

    atom_type[1] = 2;
    basis[1][0] = 2./3.;
    basis[1][1] = 2./3.;
    basis[1][2] = 0.1;

    atom_type[2] = 2;
    basis[2][0] = 2./3.;
    basis[2][1] = 2./3.;
    basis[2][2] = -0.1;

    memory->new_1d(nt,ntype);
    // nt[0] = 1;
    // nt[1] = 2;
    elemCount.push_back(1);
    elemCount.push_back(2);
}

/* ----------------------------------------------------------------------
   Initialize for FCC lattice
------------------------------------------------------------------------- */
void StructureLib::tmdc_2h(double i_alat, std::vector<std::string> elements)
{

    species.push_back(elements[0]);
    species.push_back(elements[1]);
    alat = blat = clat =  i_alat;
    char str[MAX_STRING_LENGTH];
    
    memory->new_1d(name,9);
    sprintf(str,"%s%s2-2H",elements[0].c_str(),elements[1].c_str());
    strcpy(name, str);

    apcell = 3;
    ntype  = 2;

    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    a2.push_back(0.5);
    a2.push_back(sqrt(0.75));
    a2.push_back(0.);

    a3.push_back(0.);
    a3.push_back(0.);
    a3.push_back(4.6);

    memory->new_2d(basis,apcell,3);
    memory->new_1d(atom_type,apcell);


    atom_type[0] = 1;
    basis[0][0] = 1./3.;
    basis[0][1] = 1./3.;
    basis[0][2] = 0.;

    atom_type[1] = 2;
    basis[1][0] = 2./3.;
    basis[1][1] = 2./3.;
    basis[1][2] = 0.1;

    atom_type[2] = 2;
    basis[2][0] = 2./3.;
    basis[2][1] = 2./3.;
    basis[2][2] = -0.1;

    memory->new_1d(nt,ntype);
    // nt[0] = 1;
    // nt[1] = 2;
    elemCount.push_back(1);
    elemCount.push_back(2);
}

/* ----------------------------------------------------------------------
   Initialize for FCC lattice
------------------------------------------------------------------------- */
void StructureLib::tmdc_1t(double i_alat, std::vector<std::string> elements)
{
    species.push_back(elements[0]);
    species.push_back(elements[1]);
    alat = blat = clat =  i_alat;
    char str[MAX_STRING_LENGTH];

    
    memory->new_1d(name,9);

    sprintf(str,"%s%s2-1T",elements[0].c_str(),elements[1].c_str());
    strcpy(name, str);

    apcell = 3;
    ntype  = 2;

    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    a2.push_back(0.5);
    a2.push_back(sqrt(0.75));
    a2.push_back(0.);

    a3.push_back(0.);
    a3.push_back(0.);
    a3.push_back(4.6);

    memory->new_2d(basis,apcell,3);
    memory->new_1d(atom_type,apcell);

    atom_type[0] = 1;

    basis[0][0] = 1.0;
    basis[0][1] = 0.0;
    basis[0][2] = 0.;

    atom_type[1] = 2;
    basis[1][0] = 1./3.;
    basis[1][1] = 1./3.;
    basis[1][2] = 0.1;

    atom_type[2] = 2;
    basis[2][0] = 2./3.;
    basis[2][1] = 2./3.;
    basis[2][2] = -0.1;

    memory->new_1d(nt,ntype);
    // nt[0] = 1;
    // nt[1] = 2;
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
    
    memory->new_1d(name,9);
    strcpy(name, "PETN-I");

    apcell = n1+n2+n3+n4;
    ntype  = 4;

    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    a2.push_back(0.);
    a2.push_back(1.);
    a2.push_back(0.);

    a3.push_back(0.);
    a3.push_back(0.);
    a3.push_back(1.);
    
    memory->new_2d(basis,apcell,3);
    memory->new_1d(atom_type,apcell);

    int i;

    // assign atomic types to an integral type
    // used for LAMMPS format
    for (i=0; i<n1; i++) {atom_type[i] = 1;}
    for (i=n1;i<(n1+n2);i++) {atom_type[i] = 2;}
    for (i=(n1+n2);i<(n1+n2+n3);i++) {atom_type[i] = 3;}
    for (i=(n1+n2+n3);i<(n1+n2+n3+n4);i++) {atom_type[i] = 4;}


    //10 carbons
    basis[0][0] = 0.1234250102952819;
    basis[0][1] = 0.0609475779157065;
    basis[0][2] = 0.1292474135645465;

    basis[1][0] = 0.0000088433753593;
    basis[1][1] = 0.0000312938510123;
    basis[1][2] = -0.0001289387434683;

    basis[2][0] = 0.9390663284801790;
    basis[2][1] = 0.1234761985360032;
    basis[2][2] = 0.8705806396970199;

    basis[3][0] = 0.8765258160263396;
    basis[3][1] = 0.9390840794302127;
    basis[3][2] = 0.1292158064526805;

    basis[4][0] = 0.0609647543753718;
    basis[4][1] = 0.8765622295947940;
    basis[4][2] = 0.8705239110923606;

    basis[5][0] = 0.3765353995380687;
    basis[5][1] = 0.5609880560834769;
    basis[5][2] = 0.3704004486282517;

    basis[6][0] = 0.4999903041211348;
    basis[6][1] = 0.5000668460980627;
    basis[6][2] = 0.4997718346014457;

    basis[7][0] = 0.6234774974297089;
    basis[7][1] = 0.4390783659028797;
    basis[7][2] = 0.3704736564623982;

    basis[8][0] = 0.4390251236940225;
    basis[8][1] = 0.3766150309860417;
    basis[8][2] = 0.6291470750337455;

    basis[9][0] = 0.5609529045302394;
    basis[9][1] = 0.6234468079461685;
    basis[9][2] = 0.6291893809594302;


    //16 hydrogens
    basis[10][0] = 0.2166505711833645;
    basis[10][1] = 0.0866564632402752;
    basis[10][2] = 0.0337873970829672;

    basis[11][0] = 0.0887758460757637;
    basis[11][1] = 0.1582950926693606;
    basis[11][2] = 0.2102739712043039;

    basis[12][0] = 0.8417660599147323;
    basis[12][1] = 0.0887400617900277;
    basis[12][2] = 0.7895813768753572;    

    basis[13][0] = 0.9133244844422516;
    basis[13][1] = 0.2166442882067104;
    basis[13][2] = 0.9660899732148307;

    basis[14][0] = 0.7833440948473026;
    basis[14][1] = 0.9134255999645062;
    basis[14][2] = 0.0336869631183990;

    basis[15][0] = 0.9112456237722653;
    basis[15][1] = 0.8417278256863617;
    basis[15][2] = 0.2101494303404918;

    basis[16][0] = 0.1583317521861234;
    basis[16][1] = 0.9111470674339939;
    basis[16][2] = 0.7894997797847128;

    basis[17][0] = 0.0866175890755754;
    basis[17][1] = 0.7834053867075369;
    basis[17][2] = 0.9660981724840787;

    basis[18][0] = 0.3417191249456449;
    basis[18][1] = 0.4112951208598201;
    basis[18][2] = 0.7103029580539523;

    basis[19][0] = 0.4133054984215648;
    basis[19][1] = 0.2835143058259915;
    basis[19][2] = 0.5335140130472596;

    basis[20][0] = 0.2832771135038326;
    basis[20][1] = 0.5866293773615049;
    basis[20][2] = 0.4658365263556618;    

    basis[21][0] = 0.4111949195084212;
    basis[21][1] = 0.6583577812463978;
    basis[21][2] = 0.2895000318260154;

    basis[22][0] = 0.7166258796365181;
    basis[22][1] = 0.4133987105547882;
    basis[22][2] = 0.4660511557368610;

    basis[23][0] = 0.5888023187202147;
    basis[23][1] = 0.3416798637707668;
    basis[23][2] = 0.2896045487611044;

    basis[24][0] = 0.6582972030433654;
    basis[24][1] = 0.5887284539207753;
    basis[24][2] = 0.7101663992051054;

    basis[25][0] = 0.5866920614111345;
    basis[25][1] = 0.7166589624675711;
    basis[25][2] = 0.5338202257870298;


    //8 nitrogens
    basis[26][0] = 0.2947347703961428;
    basis[26][1] = 0.9821515438346622;
    basis[26][2] = 0.3797160234100750;

    basis[27][0] = 0.0178294940200101;
    basis[27][1] = 0.2948505339720639;
    basis[27][2] = 0.6201136715821307;

    basis[28][0] = 0.7051108359660473;
    basis[28][1] = 0.0178089851520490;
    basis[28][2] = 0.3798864699252137;

    basis[29][0] = 0.9821978363821821;
    basis[29][1] = 0.7051934528440589;
    basis[29][2] = 0.6199950129470895;

    basis[30][0] = 0.2051559147175749;
    basis[30][1] = 0.4821873037409990;
    basis[30][2] = 0.1199689528250137;       

    basis[31][0] = 0.7949105514747270;
    basis[31][1] = 0.5177725552758387;
    basis[31][2] = 0.1199376239742951;    

    basis[32][0] = 0.5179199447291160;
    basis[32][1] = 0.2050940824513886;
    basis[32][2] = 0.8796389829638788;

    basis[33][0] = 0.4821452680523884;
    basis[33][1] = 0.7947528735792229;
    basis[33][2] = 0.8797900233851840;

    //24 oxygens
    basis[34][0] = 0.1618852163539675;
    basis[34][1] = 0.9510081206349513;
    basis[34][2] = 0.2757780650712896;

    basis[35][0] = 0.3227915388516672;
    basis[35][1] = 0.8914294080478391;
    basis[35][2] = 0.5073223144091146;

    basis[36][0] = 0.3602029628269364;
    basis[36][1] = 0.0901357092857124;
    basis[36][2] = 0.3302479620730800;

    basis[37][0] = 0.0490072852211034;
    basis[37][1] = 0.1620140936696899;
    basis[37][2] = 0.7240555554977974;

    basis[38][0] = 0.8379628543087850;
    basis[38][1] = 0.0489652805247084;
    basis[38][2] = 0.2758006459411306;

    basis[39][0] = 0.9510664982713117;
    basis[39][1] = 0.8380481950095333;
    basis[39][2] = 0.7239560410678860;

    basis[40][0] = 0.1085867467259578;
    basis[40][1] = 0.3229244499835243;
    basis[40][2] = 0.4925138229733992;

    basis[41][0] = 0.9098154857469101;
    basis[41][1] = 0.3602828797252814;
    basis[41][2] = 0.6695602499555666;

    basis[42][0] = 0.6771102636747253;
    basis[42][1] = 0.1085782211312506;
    basis[42][2] = 0.5074552651456001;

    basis[43][0] = 0.6396155287461972;
    basis[43][1] = 0.9098266726469774;
    basis[43][2] = 0.3305749330556768;

    basis[44][0] = 0.8914621886736926;
    basis[44][1] = 0.6771815144293790;
    basis[44][2] = 0.4923903682838448;

    basis[45][0] = 0.0901625264121581;
    basis[45][1] = 0.6396707938975306;
    basis[45][2] = 0.6694141140483759;

    basis[46][0] = 0.3380730315086735;
    basis[46][1] = 0.4511217568637719;
    basis[46][2] = 0.2237683802338916;

    basis[47][0] = 0.1771333054012358;
    basis[47][1] = 0.3914192271522535;
    basis[47][2] = 0.9924133032541617;

    basis[48][0] = 0.1396348998680058;
    basis[48][1] = 0.5901324556449838;
    basis[48][2] = 0.1694633332009857;

    basis[49][0] = 0.6620266890603006;
    basis[49][1] = 0.5489450552701405;
    basis[49][2] = 0.2238694482960191;

    basis[50][0] = 0.8229479022571921;
    basis[50][1] = 0.6084903313233199;
    basis[50][2] = 0.9922922798255897;

    basis[51][0] = 0.8603795173190565;
    basis[51][1] = 0.4098048941058630;
    basis[51][2] = 0.1694526029115362;

    basis[52][0] = 0.5489743130018822;
    basis[52][1] = 0.3380205341087155;
    basis[52][2] = 0.7756134800540011;

    basis[53][0] = 0.6086810621120478;
    basis[53][1] = 0.1771109719520004;
    basis[53][2] = 0.0072177208410251;

    basis[54][0] = 0.4099954240810333;
    basis[54][1] = 0.1395464919568412;
    basis[54][2] = 0.8302308898806596;    

    basis[55][0] = 0.4509832618773044;
    basis[55][1] = 0.6619671914581469;
    basis[55][2] = 0.7757170846853854;   

    basis[56][0] = 0.3914990872287507;
    basis[56][1] = 0.8226488271826732;
    basis[56][2] = 0.0076379856097428;

    basis[57][0] = 0.5900556737878734;
    basis[57][1] = 0.8603147456994008;
    basis[57][2] = 0.8302824086065014;


    memory->new_1d(nt,ntype);
    // nt[0] = n1;
    // nt[1] = n2;
    // nt[2] = n3;
    // nt[3] = n4;

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
    
    memory->new_1d(name,9);
    strcpy(name, "TATB");

    apcell = n1+n2+n3+n4;
    ntype  = 4;

    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    a2.push_back(-0.5);
    a2.push_back(sqrt(3.)*0.5);
    a2.push_back(0.);

    a3.push_back(-0.0240118469);
    a3.push_back(-0.2919282894);
    a3.push_back(0.6970015390);
    
    memory->new_2d(basis,apcell,3);
    memory->new_1d(atom_type,apcell);

    int i;

    // assign atomic types to an integral type
    // used for LAMMPS format
    for (i=0; i<n1; i++) {atom_type[i] = 1;}
    for (i=n1;i<(n1+n2);i++) {atom_type[i] = 2;}
    for (i=(n1+n2);i<(n1+n2+n3);i++) {atom_type[i] = 3;}
    for (i=(n1+n2+n3);i<(n1+n2+n3+n4);i++) {atom_type[i] = 4;}


    //12 carbons
    basis[0][0] = 0.533225421078354;
    basis[0][1] = 0.165145954859975;
    basis[0][2] = 0.256847799181831;

    basis[1][0] = 0.373304211034811;
    basis[1][1] = 0.002585335830150;
    basis[1][2] = 0.248726759369562;

    basis[2][0] = 0.214961944984982;
    basis[2][1] = 0.007316285568772;
    basis[2][2] = 0.248726759369562;

    basis[3][0] = 0.214427929277325;
    basis[3][1] = 0.166670338807011;
    basis[3][2] = 0.251115298308347;

    basis[4][0] = 0.376025277890588;
    basis[4][1] = 0.321767502409863;
    basis[4][2] = 0.244268149666836;

    basis[5][0] = 0.537898646133822;
    basis[5][1] = 0.326821039629845;
    basis[5][2] = 0.252070702514404;

    basis[6][0] = 0.466774595694231;
    basis[6][1] = 0.834854029550889;
    basis[6][2] = 0.743152164596877;

    basis[7][0] = 0.626695782887236;
    basis[7][1] = 0.997414655007152;
    basis[7][2] = 0.751273223517474;

    basis[8][0] = 0.785038044140087;
    basis[8][1] = 0.992683682380962;
    basis[8][2] = 0.751273223517474;

    basis[9][0] = 0.785572078788985;
    basis[9][1] = 0.833329666074985;
    basis[9][2] = 0.748884703527783;

    basis[10][0] = 0.623974722821893;
    basis[10][1] = 0.678232489598502;
    basis[10][2] = 0.755731814271107;

    basis[11][0] = 0.462101351628306;
    basis[11][1] = 0.673178958879267;
    basis[11][2] = 0.747929280372632;


    //12 hydrogens
    basis[12][0] = 0.467025165613341;
    basis[12][1] = -0.142931246616431;
    basis[12][2] = 0.238057937215777;    

    basis[13][0] = 0.280042698019573;
    basis[13][1] = 0.231949920015825;
    basis[13][2] = 0.260032517713798;

    basis[14][0] = -0.017031243524412;
    basis[14][1] = 0.091044063578516;
    basis[14][2] = 0.253981548824707;

    basis[15][0] = 0.070986996692684;
    basis[15][1] = 0.287953862359566;
    basis[15][2] = 0.253981548824707;

    basis[16][0] = 0.681930308081900;
    basis[16][1] = 0.563961747642050;
    basis[16][2] = 0.243949662702136;

    basis[17][0] = 0.778996742979354;
    basis[17][1] = 0.453953817371074;
    basis[17][2] = 0.224045152968056;

    basis[18][0] = 0.532974862646119;
    basis[18][1] = 1.142931253968417;
    basis[18][2] = 0.761942026722166;

    basis[19][0] = 0.719957288417477;
    basis[19][1] = 1.231949896808433;
    basis[19][2] = 0.739967446224145;

    basis[20][0] = 1.017031219499684;
    basis[20][1] = 0.908955931094691;
    basis[20][2] = 0.746018434062330;    

    basis[21][0] = 0.929012981138629;
    basis[21][1] = 0.712046136021683;
    basis[21][2] = 0.746018434062330;

    basis[22][0] = 0.318069693261142;
    basis[22][1] = 0.436038257112083;
    basis[22][2] = 0.756050339133994;

    basis[23][0] = 0.221003277337608;
    basis[23][1] = 0.546046172295165;
    basis[23][2] = 0.775954848868074;


    // 12 nitrogens
    basis[24][0] = 0.692005882299451;
    basis[24][1] = 0.168225920727732;
    basis[24][2] = 0.269427448696825;

    basis[25][0] = 0.369300808671666;
    basis[25][1] = -0.146627249648234;
    basis[25][2] = 0.238854116808960;

    basis[26][0] = 0.056459867784244;
    basis[26][1] = -0.149373393885893;
    basis[26][2] = 0.246656688605621;

    basis[27][0] = 0.070985482661491;
    basis[27][1] = 0.172244042682571;
    basis[27][2] = 0.257803203387888;

    basis[28][0] = 0.374694065413529;
    basis[28][1] = 0.476317110052965;
    basis[28][2] = 0.235032443137448;

    basis[29][0] = 0.684170743759448;
    basis[29][1] = 0.470943956289962;
    basis[29][2] = 0.251752234339561;

    basis[30][0] = 0.307994116357621;
    basis[30][1] = 0.831774040795564;
    basis[30][2] = 0.730572515081882;       

    basis[31][0] = 0.630699181083965;
    basis[31][1] = 1.146627230343737;
    basis[31][2] = 0.761145903976264;    

    basis[32][0] = 0.943540136748140;
    basis[32][1] = 1.149373392395005;
    basis[32][2] = 0.753343294281416;

    basis[33][0] = 0.929014504299457;
    basis[33][1] = 0.827755968572310;
    basis[33][2] = 0.742196817397335;

    basis[34][0] = 0.625305927716160;
    basis[34][1] = 0.523682866813952;
    basis[34][2] = 0.764967520641259;

    basis[35][0] = 0.315829242757960;
    basis[35][1] = 0.529056020449092;
    basis[35][2] = 0.748247729439146;

    //12 oxygens
    basis[36][0] = 0.833500138561252;
    basis[36][1] = 0.306841203868823;
    basis[36][2] = 0.282007098371055;

    basis[37][0] = 0.692721923306593;
    basis[37][1] = 0.029772561755543;
    basis[37][2] = 0.264172659400916;

    basis[38][0] = 0.051178846281235;
    basis[38][1] = -0.291871478373279;
    basis[38][2] = 0.237739469200169;

    basis[39][0] = -0.081025791661196;
    basis[39][1] = -0.149821310399948;
    basis[39][2] = 0.252548414091980;

    basis[40][0] = 0.238007049519439;
    basis[40][1] = 0.478855668304046;
    basis[40][2] = 0.230414608981084;

    basis[41][0] = 0.513048663412336;
    basis[41][1] = 0.617436912135891;
    basis[41][2] = 0.235669398276992;

    basis[42][0] = 0.166499841327518;
    basis[42][1] = 0.693158772923784;
    basis[42][2] = 0.717992865566888;

    basis[43][0] = 0.307278090788424;
    basis[43][1] = 0.970227430380685;
    basis[43][2] = 0.735827304537027;

    basis[44][0] = 0.948821173076783;
    basis[44][1] = 1.291871504897470;
    basis[44][2] = 0.762260551744290;

    basis[45][0] = 1.081025785381034;
    basis[45][1] = 1.149821336924139;
    basis[45][2] = 0.747451606852479;

    basis[46][0] = 0.761992928968073;
    basis[46][1] = 0.521144327788630;
    basis[46][2] = 0.769585411963375;

    basis[47][0] = 0.486951344458807;
    basis[47][1] = 0.382563091447179;
    basis[47][2] = 0.764330622508231;


    memory->new_1d(nt,ntype);
    // nt[0] = n1;
    // nt[1] = n2;
    // nt[2] = n3;
    // nt[3] = n4;

    elemCount.push_back(n1);
    elemCount.push_back(n2);
    elemCount.push_back(n3);
    elemCount.push_back(n4);

}
