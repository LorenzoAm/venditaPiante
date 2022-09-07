#pragma once

#include <stdbool.h>
#include <mysql.h>

struct configuration {
	char* host;
	char* db_username;
	char* db_password;
	unsigned int port;
	char* database;

	char username[12];
	char password[12];
};

extern struct configuration conf;

extern int parse_config(char* path, struct configuration* conf);
extern char* strndup(const char* s, size_t n);
extern bool yesOrNo(char* domanda, char yes, char no, bool predef, bool insensitive);
extern void print_error(MYSQL* conn, char* message);
extern void print_stmt_error(MYSQL_STMT* stmt, char* message);
extern void finish_with_error(MYSQL* conn, char* message);
extern void finish_with_stmt_error(MYSQL* conn, MYSQL_STMT* stmt, char* message, bool close_stmt);
extern bool setup_prepared_stmt(MYSQL_STMT** stmt, char* statement, MYSQL* conn);
extern void dump_result_set(MYSQL* conn, MYSQL_STMT* stmt, char* title);
extern void run_as_manager(MYSQL* conn);
extern void run_as_warehouse_operator(MYSQL* conn);
extern void add_supply(MYSQL* conn);
extern void add_supplier(MYSQL* conn);
extern void view_suppliers(MYSQL* conn);
extern void view_private_customers(MYSQL* conn);
extern void view_shop_customers(MYSQL* conn);
extern void add_private_customer(MYSQL* conn);
extern void add_shop_customer(MYSQL* conn);
extern void add_pack(MYSQL* conn);
extern void add_shopOrder(MYSQL* conn);
extern void add_plant_pack(MYSQL* conn);
extern void add_price(MYSQL* conn);
extern void add_privateOrder(MYSQL* conn);
extern void add_orderedPlant(MYSQL* conn);
extern void view_supplies(MYSQL* conn);
extern void view_plants(MYSQL* conn);
extern void add_treated_plant(MYSQL* conn);
extern void view_private_orders(MYSQL* conn);
extern void view_shop_orders(MYSQL* conn);
extern void delete_order(MYSQL* conn);
extern void complete_orders(MYSQL* conn);
extern void view_plants(MYSQL* conn);
extern void view_number_packages(MYSQL* conn);
extern void view_packages_info(MYSQL* conn);
extern void add_plant(MYSQL* conn);