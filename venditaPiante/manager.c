#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"

void run_as_manager(MYSQL* conn)
{

	int op;

	printf("\n\n- Hai effettuato l'accesso come manager! -\n");

	if (!parse_config("manager.json", &conf)) {
		fprintf(stderr, "Unable to load manager configuration\n");
		exit(EXIT_FAILURE);
	}

	if (mysql_change_user(conn, conf.db_username, conf.db_password, conf.database)) {
		fprintf(stderr, "mysql_change_user() failed\n");
		exit(EXIT_FAILURE);
	}

	do
	{
		printf("\n\n*** Scegli un'operazione... ***\n\n");
		printf("1) Cancella ordine\n");
		printf("2) Completa ordini\n");
		printf("3) Visualizza piante\n");
		printf("4) Visualizza privati\n");
		printf("5) Visualizza rivendite\n");
		printf("6) Inserisci ordine privato\n");
		printf("7) Inserisci ordine rivendita\n");
		printf("8) Inserisci pacco\n");
		printf("9) Inserisci pianta\n");
		printf("10) Inserisci privato\n");
		printf("11) Inserisci rivendita\n");
		printf("12) Inserisci pianta nel pacco\n");
		printf("13) Inserisci nuovo prezzo pianta\n");
		printf("14) Visualizza ordini privati\n");
		printf("15) Visualizza ordini rivendite\n");
		printf("16) Visualizza numero pacchi ordine\n");
		printf("17) Visualizza info pacchi ordine\n");
		printf("18) Inserisci riferimento pianta per un ordine\n");
		printf("19) Esci\n\n");


		scanf("%d", &op);

		switch (op)
		{
		case 1:
			delete_order(conn);
			break;
		case 2:
			complete_orders(conn);
			break;
		case 3:
			view_plants(conn);
			break;
		case 4:
			view_private_customers(conn);
			break;
		case 5:
			view_shop_customers(conn);
			break;
		case 6:
			add_privateOrder(conn);
			break;
		case 7:
			add_shopOrder(conn);
			break;
		case 8:
			add_pack(conn);
			break;
		case 9:
			add_plant(conn);
			break;
		case 10:
			add_private_customer(conn);
			break;
		case 11:
			add_shop_customer(conn);
			break;
		case 12:
			add_plant_pack(conn);
			break;
		case 13:
			add_price(conn);
			break;
		case 14:
			view_private_orders(conn);
			break;
		case 15:
			view_shop_orders(conn);
			break;
		case 16:
			view_number_packages(conn);
			break;
		case 17:
			view_packages_info(conn);
			break;
		case 18:
			add_orderedPlant(conn);
			break;
		case 19:
			break;


		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
		}

		//getchar();
	} while (op != 19);
}

static void view_private_customers(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;

	printf("\n Lista privati : \n");


	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call DatiPrivati()", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize private customers list statement\n", false);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while retrieving the private customers list.");
	}

	dump_result_set(conn, prepared_stmt, "");

	mysql_stmt_next_result(prepared_stmt);


	mysql_stmt_close(prepared_stmt);
}

static void view_shop_customers(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;

	printf("\n Lista rivendite : \n");


	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call DatiRivendite()", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize shop customers list statement\n", false);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while retrieving the shop customers list.");
	}

	dump_result_set(conn, prepared_stmt, "");

	mysql_stmt_next_result(prepared_stmt);


	mysql_stmt_close(prepared_stmt);
}

static void add_private_customer(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[9];

	char nome[45];
	char cognome[45];
	char cf[20];
	char via[45];
	char civico[10];
	char citt‡[45];
	char viaFatt[45];
	char civicoFatt[10];
	char citt‡Fatt[45];

	

	getchar();

	printf("\nCF: ");
	fflush(stdout);
	fgets(cf, 20, stdin);
	cf[strlen(cf) - 1] = '\0';

	printf("\nNome: ");
	fflush(stdout);
	fgets(nome, 45, stdin);
	nome[strlen(nome) - 1] = '\0';

	printf("\nCognome: ");
	fflush(stdout);
	fgets(cognome, 45, stdin);
	cognome[strlen(cognome) - 1] = '\0';

	printf("\nVia: ");
	fflush(stdout);
	fgets(via, 45, stdin);
	via[strlen(via) - 1] = '\0';

	printf("\nCivico: ");
	fflush(stdout);
	fgets(civico, 10, stdin);
	civico[strlen(civico) - 1] = '\0';

	printf("\nCitta': ");
	fflush(stdout);
	fgets(citt‡, 45, stdin);
	citt‡[strlen(citt‡) - 1] = '\0';

	printf("\nVia fatturazione: ");
	fflush(stdout);
	fgets(viaFatt, 45, stdin);
	viaFatt[strlen(viaFatt) - 1] = '\0';

	printf("\nCivico fatturazione: ");
	fflush(stdout);
	fgets(civicoFatt, 10, stdin);
	civicoFatt[strlen(civicoFatt) - 1] = '\0';

	printf("\nCitta' fatturazione: ");
	fflush(stdout);
	fgets(citt‡Fatt, 45, stdin);
	citt‡Fatt[strlen(citt‡Fatt) - 1] = '\0';

	


	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call NuovoPrivato(?, ?, ?, ?, ?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize private customer insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = cf;
	param[0].buffer_length = strlen(cf);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = nome;
	param[1].buffer_length = strlen(nome);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = cognome;
	param[2].buffer_length = strlen(cognome);

	param[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[3].buffer = via;
	param[3].buffer_length = strlen(via);

	param[4].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[4].buffer = civico;
	param[4].buffer_length = strlen(civico);

	param[5].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[5].buffer = citt‡;
	param[5].buffer_length = strlen(citt‡);

	param[6].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[6].buffer = viaFatt;
	param[6].buffer_length = strlen(viaFatt);

	param[7].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[7].buffer = civicoFatt;
	param[7].buffer_length = strlen(civicoFatt);

	param[8].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[8].buffer = citt‡Fatt;
	param[8].buffer_length = strlen(citt‡Fatt);




	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Could not bind parameters for private customer insertion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while adding the private customer.");
	}
	else {
		printf("\n Privato inserito correttamente...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void add_shop_customer(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[10];

	char nome[45];
	char iva[15];
	char via[45];
	char civico[10];
	char citt‡[45];
	char viaFatt[45];
	char civicoFatt[10];
	char citt‡Fatt[45];
	char nomeRef[45];
	char cognomeRef[45];

	
	getchar();

	printf("\nPartitaIVA: ");
	fflush(stdout);
	fgets(iva, 15, stdin);
	iva[strlen(iva) - 1] = '\0';

	printf("\nNome: ");
	fflush(stdout);
	fgets(nome, 45, stdin);
	nome[strlen(nome) - 1] = '\0';

	printf("\nVia: ");
	fflush(stdout);
	fgets(via, 45, stdin);
	via[strlen(via) - 1] = '\0';

	printf("\nCivico: ");
	fflush(stdout);
	fgets(civico, 10, stdin);
	civico[strlen(civico) - 1] = '\0';

	printf("\nCitta': ");
	fflush(stdout);
	fgets(citt‡, 45, stdin);
	citt‡[strlen(citt‡) - 1] = '\0';

	printf("\nVia fatturazione: ");
	fflush(stdout);
	fgets(viaFatt, 45, stdin);
	viaFatt[strlen(viaFatt) - 1] = '\0';

	printf("\nCivico fatturazione: ");
	fflush(stdout);
	fgets(civicoFatt, 10, stdin);
	civicoFatt[strlen(civicoFatt) - 1] = '\0';

	printf("\nCitta' fatturazione: ");
	fflush(stdout);
	fgets(citt‡Fatt, 45, stdin);
	citt‡Fatt[strlen(citt‡Fatt) - 1] = '\0';

	printf("\nNome referente: ");
	fflush(stdout);
	fgets(nomeRef, 45, stdin);
	nomeRef[strlen(nomeRef) - 1] = '\0';

	printf("\nCognome referente: ");
	fflush(stdout);
	fgets(cognomeRef, 45, stdin);
	cognomeRef[strlen(cognomeRef) - 1] = '\0';




	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call NuovaRivendita(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize shop customer insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = iva;
	param[0].buffer_length = strlen(iva);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = nome;
	param[1].buffer_length = strlen(nome);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = via;
	param[2].buffer_length = strlen(via);

	param[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[3].buffer = civico;
	param[3].buffer_length = strlen(civico);

	param[4].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[4].buffer = citt‡;
	param[4].buffer_length = strlen(citt‡);

	param[5].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[5].buffer = viaFatt;
	param[5].buffer_length = strlen(viaFatt);

	param[6].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[6].buffer = civicoFatt;
	param[6].buffer_length = strlen(civicoFatt);

	param[7].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[7].buffer = citt‡Fatt;
	param[7].buffer_length = strlen(citt‡Fatt);

	param[8].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[8].buffer = nomeRef;
	param[8].buffer_length = strlen(nomeRef);

	param[9].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[9].buffer = cognomeRef;
	param[9].buffer_length = strlen(cognomeRef);




	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Could not bind parameters for shop customer insertion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while adding the shop customer.");
	}
	else {
		printf("\nRivendita inserita correttamente...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void view_private_orders(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;


	printf("\n Lista ordini privati: \n");


	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call OrdiniPrivati()", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize private customers orders list statement\n", false);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while retrieving the private customers orders list.");
	}

	dump_result_set(conn, prepared_stmt, "");

	mysql_stmt_next_result(prepared_stmt);


	mysql_stmt_close(prepared_stmt);
}

static void view_shop_orders(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;

	printf("\n Lista ordini rivendite: \n");


	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call OrdiniRivendite()", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize shop customers orders list statement\n", false);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while retrieving the shop customers orders list.\n");
	}

	dump_result_set(conn, prepared_stmt, "");

	mysql_stmt_next_result(prepared_stmt);


	mysql_stmt_close(prepared_stmt);
}

static void delete_order(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[1];

	int codice;

	printf("\nCodice ordine: ");
	scanf("%d", &codice);


	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call CancellaOrdine(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize order delete statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &codice;
	param[0].buffer_length = sizeof(codice);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Could not bind parameters for order deletion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while deleting the order.");
	}
	else {
		printf("\nOrdine cancellato correttamente...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void complete_orders(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;

	printf("\n Ordini da completare: \n");


	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call CompletaOrdini()", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize incompleted orders list statement\n", false);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while retrieving the incompleted orders list.\n");
	}

	dump_result_set(conn, prepared_stmt, "");

	mysql_stmt_next_result(prepared_stmt);


	mysql_stmt_close(prepared_stmt);
}

static void view_plants(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;

	printf("\n Lista piante: \n");


	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call DatiPiante()", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize plants list statement\n", false);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while retrieving the plants list.\n");
	}

	dump_result_set(conn, prepared_stmt, "");

	mysql_stmt_next_result(prepared_stmt);


	mysql_stmt_close(prepared_stmt);
}

static void view_number_packages(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[1];

	int codice;

	

	printf("\n Codice ordine: ");
	scanf("%d",&codice);

	printf("\n Lista pacchi ordine: \n");

	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call NumeroPacchiOrdine(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize order's packages list statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &codice;
	param[0].buffer_length = sizeof(codice);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Could not bind parameters for order deletion\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while retrieving the order's packages list.\n");
	}

	dump_result_set(conn, prepared_stmt, "");

	mysql_stmt_next_result(prepared_stmt);


	mysql_stmt_close(prepared_stmt);
}

static void view_packages_info(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[1];

	int codice = 0;

	printf("\nCodice ordine: ");
	scanf("%d",&codice);

	printf("\n Info pacchi ordine: \n");


	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call QuantitaPiantePacchi(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize order's packages list statement\n", false);
	}
	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &codice;
	param[0].buffer_length = sizeof(codice);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Could not bind parameters for order deletion\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while retrieving the order's packages list.\n");
	}

	dump_result_set(conn, prepared_stmt, "");

	mysql_stmt_next_result(prepared_stmt);


	mysql_stmt_close(prepared_stmt);
}

static void add_plant(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[5];
	struct MYSQL_TIME data;

	char codice[45];
	char nomeComune[45];
	char nomeLatino[45];
	char giornoStr[45];
	char meseStr[45];
	char annoStr[45];
	float prezzo = 0.0;

	
	getchar();

	printf("\nCodice pianta: ");
	fflush(stdout);
	fgets(codice, 45, stdin);
	codice[strlen(codice) - 1] = '\0';

	printf("\nNome comune: ");
	fflush(stdout);
	fgets(nomeComune, 45, stdin);
	nomeComune[strlen(nomeComune) - 1] = '\0';

	printf("\nNome latino: ");
	fflush(stdout);
	fgets(nomeLatino, 45, stdin);
	nomeLatino[strlen(nomeLatino) - 1] = '\0';

	printf("\nPrezzo (Euro): ");
	scanf("%f", &prezzo);

	printf("\nGiorno validita' [1-31]: ");
	scanf("%s", giornoStr);

	printf("\nMese validita' [1-12]:");
	scanf("%s", meseStr);

	printf("\nAnno validita': ");
	scanf("%s", annoStr);

	int giorno = atoi(giornoStr);
	int mese = atoi(meseStr);
	int anno = atoi(annoStr);

	data.day = giorno;
	data.month = mese;
	data.year = anno;



	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call NuovaPianta(?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize plant insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = codice;
	param[0].buffer_length = strlen(codice);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = nomeLatino;
	param[1].buffer_length = strlen(nomeLatino);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = nomeComune;
	param[2].buffer_length = strlen(nomeComune);

	param[3].buffer_type = MYSQL_TYPE_DATE;
	param[3].buffer = &data;
	param[3].buffer_length = sizeof(MYSQL_TIME);

	param[4].buffer_type = MYSQL_TYPE_FLOAT;
	param[4].buffer = &prezzo;
	param[4].buffer_length = sizeof(prezzo);




	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Could not bind parameters for plant insertion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while adding the plant.");
	}
	else {
		printf("\n Pianta inserita correttamente...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void add_pack(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[1];

	int ordine = 0;

	
	getchar();

	printf("\nCodice ordine: ");
	scanf("%d", &ordine);



	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call NuovoPaccoOrdine(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize package insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &ordine;
	param[0].buffer_length = sizeof(ordine);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Could not bind parameters for package insertion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while adding the package");
	}
	else {
		printf("\n Pacco inserito correttamente...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void add_plant_pack(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];

	char pianta[45];
	int pacco = 0;
	int qta = 0;

	getchar();

	printf("\nCodice pianta: ");
	fflush(stdout);
	fgets(pianta, 45, stdin);
	pianta[strlen(pianta) - 1] = '\0';

	printf("\nCodice pacco: ");
	scanf("%d", &pacco);

	printf("\nQuantita': ");
	scanf("%d", &qta);




	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call InserirePiantaPacco(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize plant package insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &pacco;
	param[0].buffer_length = sizeof(pacco);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = pianta;
	param[1].buffer_length = strlen(pianta);

	param[2].buffer_type = MYSQL_TYPE_LONG;
	param[2].buffer = &qta;
	param[2].buffer_length = sizeof(qta);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Could not bind parameters for plant package insertion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while adding the plant in the package");
	}
	else {
		printf("\n Pianta inserita correttamente nel pacco...\n");
	}

	mysql_stmt_close(prepared_stmt);
}


static void add_price(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];
	struct MYSQL_TIME data;

	char pianta[45];
	char giornoStr[45];
	char meseStr[45];
	char annoStr[45];
	float prezzo = 0.0;

	
	getchar();

	printf("\nCodice pianta: ");
	fflush(stdout);
	fgets(pianta, 45, stdin);
	pianta[strlen(pianta) - 1] = '\0';

	printf("\nNuovo prezzo (Euro): ");
	scanf("%f", &prezzo);

	printf("\nGiorno validita' [1-31]: ");
	scanf("%s", giornoStr);

	printf("\nMese validita' [1-12]:");
	scanf("%s", meseStr);

	printf("\nAnno validita': ");
	scanf("%s", annoStr);

	int giorno = atoi(giornoStr);
	int mese = atoi(meseStr);
	int anno = atoi(annoStr);

	data.day = giorno;
	data.month = mese;
	data.year = anno;


	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call NuovoPrezzo(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize price insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = pianta;
	param[0].buffer_length = strlen(pianta);

	param[1].buffer_type = MYSQL_TYPE_DATE;
	param[1].buffer = &data;
	param[1].buffer_length = sizeof(MYSQL_TIME);

	param[2].buffer_type = MYSQL_TYPE_FLOAT;
	param[2].buffer = &prezzo;
	param[2].buffer_length = sizeof(prezzo);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Could not bind parameters for price insertion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while adding the price");
	}
	else {
		printf("\n Nuovo prezzo inserito correttamente...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void add_privateOrder(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[6];
	struct MYSQL_TIME data;

	char cf[20];
	char contatto[45];
	char via[45];
	char civico[10];
	char citt‡[45];
	char giornoStr[45];
	char meseStr[45];
	char annoStr[45];

	getchar();

	printf("\nCF: ");
	fflush(stdout);
	fgets(cf, 20, stdin);
	cf[strlen(cf) - 1] = '\0';

	printf("\nContatto: ");
	fflush(stdout);
	fgets(contatto, 45, stdin);
	contatto[strlen(contatto) - 1] = '\0';

	printf("\nVia: ");
	fflush(stdout);
	fgets(via, 45, stdin);
	via[strlen(via) - 1] = '\0';

	printf("\nCivico: ");
	fflush(stdout);
	fgets(civico, 10, stdin);
	civico[strlen(civico) - 1] = '\0';

	printf("\nCitta': ");
	fflush(stdout);
	fgets(citt‡, 45, stdin);
	citt‡[strlen(citt‡) - 1] = '\0';


	printf("\nGiorno ordinazione [1-31]: ");
	scanf("%s", giornoStr);

	printf("\nMese ordinazione [1-12]:");
	scanf("%s", meseStr);

	printf("\nAnno ordinazione: ");
	scanf("%s", annoStr);

	int giorno = atoi(giornoStr);
	int mese = atoi(meseStr);
	int anno = atoi(annoStr);

	data.day = giorno;
	data.month = mese;
	data.year = anno;



	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call NuovoOrdinePrivato(?, ?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize private order insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = cf;
	param[0].buffer_length = strlen(cf);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = via;
	param[1].buffer_length = strlen(via);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = civico;
	param[2].buffer_length = strlen(civico);

	param[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[3].buffer = citt‡;
	param[3].buffer_length = strlen(citt‡);

	param[4].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[4].buffer = contatto;
	param[4].buffer_length = strlen(contatto);

	param[5].buffer_type = MYSQL_TYPE_DATE;
	param[5].buffer = &data;
	param[5].buffer_length = sizeof(MYSQL_TIME);



	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Could not bind parameters for private order insertion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while adding the order.");
	}
	else {
		printf("\n Ordine inserito correttamente...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void add_shopOrder(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[6];
	struct MYSQL_TIME data;

	char iva[15];
	char contatto[45];
	char via[45];
	char civico[10];
	char citt‡[45];
	char giornoStr[45];
	char meseStr[45];
	char annoStr[45];

	
	getchar();

	printf("\nPartitaIVA: ");
	fflush(stdout);
	fgets(iva, 15, stdin);
	iva[strlen(iva) - 1] = '\0';

	printf("\nContatto: ");
	fflush(stdout);
	fgets(contatto, 45, stdin);
	contatto[strlen(contatto) - 1] = '\0';

	printf("\nVia: ");
	fflush(stdout);
	fgets(via, 45, stdin);
	via[strlen(via) - 1] = '\0';

	printf("\nCivico: ");
	fflush(stdout);
	fgets(civico, 10, stdin);
	civico[strlen(civico) - 1] = '\0';

	printf("\nCitta': ");
	fflush(stdout);
	fgets(citt‡, 45, stdin);
	citt‡[strlen(citt‡) - 1] = '\0';


	printf("\nGiorno ordinazione [1-31]: ");
	scanf("%s", giornoStr);

	printf("\nMese ordinazione [1-12]:");
	scanf("%s", meseStr);

	printf("\nAnno ordinazione: ");
	scanf("%s", annoStr);

	int giorno = atoi(giornoStr);
	int mese = atoi(meseStr);
	int anno = atoi(annoStr);

	data.day = giorno;
	data.month = mese;
	data.year = anno;



	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call NuovoOrdineRivendita(?, ?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize shop order insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = iva;
	param[0].buffer_length = strlen(iva);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = via;
	param[1].buffer_length = strlen(via);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = civico;
	param[2].buffer_length = strlen(civico);

	param[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[3].buffer = citt‡;
	param[3].buffer_length = strlen(citt‡);

	param[4].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[4].buffer = contatto;
	param[4].buffer_length = strlen(contatto);

	param[5].buffer_type = MYSQL_TYPE_DATE;
	param[5].buffer = &data;
	param[5].buffer_length = sizeof(MYSQL_TIME);



	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Could not bind parameters for shop order insertion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while adding the order.");
	}
	else {
		printf("\n Ordine inserito correttamente...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void add_orderedPlant(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];

	char pianta[45];
	int ordine = 0;
	int qta = 0;

	
	getchar();

	printf("\nCodice pianta: ");
	fflush(stdout);
	fgets(pianta, 45, stdin);
	pianta[strlen(pianta) - 1] = '\0';

	printf("\nCodice ordine: ");
	scanf("%d", &ordine);

	printf("\nQuantita': ");
	scanf("%d", &qta);




	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call InserirePiantaOrdine(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize ordered plant insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = pianta;
	param[0].buffer_length = strlen(pianta);

	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &ordine;
	param[1].buffer_length = sizeof(ordine);

	param[2].buffer_type = MYSQL_TYPE_LONG;
	param[2].buffer = &qta;
	param[2].buffer_length = sizeof(qta);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Could not bind parameters for ordered plant insertion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while adding the ordered plant");
	}
	else {
		printf("\n Riferimento alla pianta aggiunto correttamente...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

