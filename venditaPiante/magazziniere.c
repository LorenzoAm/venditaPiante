#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"

void run_as_warehouse_operator(MYSQL* conn)
{

	int op;

	printf("\n\n- Hai effettuato l'accesso come magazziniere! -\n");

	if (!parse_config("magazziniere.json", &conf)) {
		fprintf(stderr, "Unable to load warehouseman configuration\n");
		exit(EXIT_FAILURE);
	}

	if (mysql_change_user(conn, conf.db_username, conf.db_password, conf.database)) {
		fprintf(stderr, "mysql_change_user() failed\n");
		exit(EXIT_FAILURE);
	}

	do
	{
		printf("\n\n*** Scegli un'operazione... ***\n\n");
		printf("1) Visualizza fornitori\n");
		printf("2) Visualizza forniture\n");
		printf("3) Visualizza piante\n");
		printf("4) Inserisci fornitore\n");
		printf("5) Inserisci pianta trattata\n");
		printf("6) Inserisci fornitura\n");
		printf("7) Esci\n\n");


		scanf("%d", &op);

		switch (op)
		{
		case 1:
			view_suppliers(conn);
			break;
		case 2:
			view_supplies(conn);
			break;
		case 3:
			view_plants(conn);
			break;
		case 4:
			add_supplier(conn);
			break;
		case 5:
			add_treated_plant(conn);
			break;
		case 6:
			add_supply(conn);
			break;

		case 7:
			break;


		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
		}

		//getchar();
	} while (op != 7);
}

static void add_supply(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[4];
	struct MYSQL_TIME data;

	char fornitore[45];
	char pianta[45];
	int qta = 0;
	char giornoStr[3];
	char meseStr[3];
	char annoStr[5];

	
	getchar();

	printf("\nCodice fornitore: ");
	fflush(stdout);
	fgets(fornitore, 45, stdin);
	fornitore[strlen(fornitore) - 1] = '\0';

	printf("\nCodice pianta: ");
	fflush(stdout);
	fgets(pianta, 45, stdin);
	pianta[strlen(pianta) - 1] = '\0';

	printf("\nQuantita' pianta: ");
	scanf("%d", &qta);

	printf("\nGiorno fornitura [1-31]: ");
	scanf("%s", giornoStr);


	printf("\nMese fornitura [1-12]: ");
	scanf("%s", meseStr);

	printf("\nAnno fornitura: ");
	scanf("%s", annoStr);

	int giorno = atoi(giornoStr);
	int mese = atoi(meseStr);
	int anno = atoi(annoStr);

	data.day = giorno;
	data.month = mese;
	data.year = anno;

	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call NuovaFornitura(?, ?, ?, ?)", conn))
	{
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize supply insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = fornitore;
	param[0].buffer_length = strlen(fornitore);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = pianta;
	param[1].buffer_length = strlen(pianta);

	param[2].buffer_type = MYSQL_TYPE_LONG;
	param[2].buffer = &qta;
	param[2].buffer_length = sizeof(qta);

	param[3].buffer_type = MYSQL_TYPE_DATE;
	param[3].buffer = &data;
	param[3].buffer_length = sizeof(MYSQL_TIME);




	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Could not bind parameters for supply insertion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while adding the supply.");
	}
	else {
		printf("\n Fornitura inserita correttamente...\n");
	}

	mysql_stmt_close(prepared_stmt);
}


static void add_supplier(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];

	char cf[20];
	char nome[45];
	char codiceFornitore[45];

	
	getchar();

	printf("\nCodice fornitore: ");
	fflush(stdout);
	fgets(codiceFornitore, 45, stdin);
	codiceFornitore[strlen(codiceFornitore) - 1] = '\0';

	printf("\nCF fornitore: ");
	fflush(stdout);
	fgets(cf, 20, stdin);
	cf[strlen(cf) - 1] = '\0';

	printf("\nNome fornitore: ");
	fflush(stdout);
	fgets(nome, 45, stdin);
	nome[strlen(nome) - 1] = '\0';



	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call NuovoFornitore(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize supplier insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = codiceFornitore;
	param[0].buffer_length = strlen(codiceFornitore);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = cf;
	param[1].buffer_length = strlen(cf);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = nome;
	param[2].buffer_length = strlen(nome);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Could not bind parameters for supplier insertion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while adding the supplier.");
	}
	else {
		printf("\n Fornitore inserito correttamente...\n");
	}

	mysql_stmt_close(prepared_stmt);
}


static void view_suppliers(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;

	printf("\n Lista fornitori : \n");


	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call DatiFornitori()", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize suppliers list statement\n", false);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while retrieving the suppliers list.");
	}

	dump_result_set(conn, prepared_stmt, "");

	mysql_stmt_next_result(prepared_stmt);


	mysql_stmt_close(prepared_stmt);


}

static void view_supplies(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;

	printf("\n Lista forniture : \n");


	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call DatiForniture()", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize supplies list statement\n", false);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while retrieving the supplies list.");
	}

	dump_result_set(conn, prepared_stmt, "");

	mysql_stmt_next_result(prepared_stmt);


	mysql_stmt_close(prepared_stmt);

}

static void view_plants(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;

	printf("\n Lista piante : \n");


	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call DatiPiante()", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize plants list statement\n", false);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while retrieving the plants list.");
	}

	dump_result_set(conn, prepared_stmt, "");

	mysql_stmt_next_result(prepared_stmt);


	mysql_stmt_close(prepared_stmt);

}

static void add_treated_plant(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[2];

	
	char pianta[45];
	char codiceFornitore[45];

	getchar();

	printf("\nCodice fornitore: ");
	fflush(stdout);
	fgets(codiceFornitore, 45, stdin);
	codiceFornitore[strlen(codiceFornitore) - 1] = '\0';

	printf("\nCodice pianta: ");
	fflush(stdout);
	fgets(pianta, 45, stdin);
	pianta[strlen(pianta) - 1] = '\0';




	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call NuovaPiantaTrattata(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Unable to initialize treated plant insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = codiceFornitore;
	param[0].buffer_length = strlen(codiceFornitore);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = pianta;
	param[1].buffer_length = strlen(pianta);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\n Could not bind parameters for treated plant insertion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\n An error occurred while adding the treated plant.");
	}
	else {
		printf("\n Pianta trattata inserita correttamente...\n");
	}

	mysql_stmt_close(prepared_stmt);
}