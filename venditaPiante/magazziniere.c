#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"

void run_as_warehouse_operator(MYSQL* conn)
{

	int op;

	do
	{
		printf("\n\n*** Choose an operation... ***\n\n");
		printf("1) View all suppliers\n");
		printf("2) View all supplies\n");
		printf("3) View all plants\n");
		printf("4) Add new supplier\n");
		printf("5) Add new treated plant\n");
		printf("6) Add new supply\n");
		printf("7) Quit\n\n");


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

	// Input for the registration routine

	char fornitore[45];
	char pianta[45];
	int qta = 0;
	char giornoStr[3];
	char meseStr[3];
	char annoStr[5];

	// Get the required information

	printf("\nSupplier code: ");
	scanf("%s", fornitore);

	printf("\nPlant code: ");
	scanf("%s", pianta);

	printf("\nPlant quantity: ");
	scanf("%d", qta);

	printf("\nSupply day [1-31]: ");
	scanf("%s", giornoStr);


	printf("\nSupply month [1-12]:");
	scanf("%s", meseStr);

	printf("\nSupply year: ");
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
		printf("\n Supply correctly added...\n");
	}

	mysql_stmt_close(prepared_stmt);
}


static void add_supplier(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];

	// Input for the registration routine
	char cf[45];
	char nome[45];
	char codiceFornitore[45];

	// Get the required information

	printf("\nSupplier code: ");
	scanf("%s", codiceFornitore);

	printf("\nSupplier cf: ");
	scanf("%s", cf);

	printf("\nSupplier name: ");
	scanf("%s", nome);



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
		printf("\n Supplier correctly added...\n");
	}

	mysql_stmt_close(prepared_stmt);
}


static void view_suppliers(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;

	printf("\n Suppliers list : \n");


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

	printf("\n Supplies list : \n");


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

	printf("\n Plants list : \n");


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

	// Input for the registration routine
	char pianta[45];
	char codiceFornitore[45];

	// Get the required information

	printf("\nSupplier code: ");
	scanf("%s", codiceFornitore);

	printf("\nPlant code: ");
	scanf("%s", pianta);




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
		printf("\n treated plant correctly added...\n");
	}

	mysql_stmt_close(prepared_stmt);
}