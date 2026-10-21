// Librerias y configuracion
#include <algorithm>
#include <ctime>
#include <direct.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

using namespace std;

// Habilitar secuencias de escape ANSI en la terminal
void activarTerminal() {
#ifdef _WIN32
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD dwMode = 0;
  if (GetConsoleMode(hOut, &dwMode)) {
    SetConsoleMode(hOut, dwMode | 0x0004); // ENABLE_VIRTUAL_TERMINAL_PROCESSING
  }
#endif
}
// Utilidades de formato y colores para terminal
void imprimirTitulo(const string &titulo) {
  cout << "\n\033[96m============================================================\033[0m" << endl;
  cout << "  \033[97;1m" << titulo << "\033[0m" << endl;
  cout << "\033[96m============================================================\033[0m" << endl;
}

void imprimirSeparador() {
  cout << "\033[90m------------------------------------------------------------\033[0m" << endl;
}

void mensajeExito(const string &msg) {
  cout << "\n\033[92;1m[+] " << msg << "\033[0m" << endl;
}

void mensajeError(const string &msg) {
  cout << "\n\033[91;1m[!] " << msg << "\033[0m" << endl;
}

void mensajeAlerta(const string &msg) {
  cout << "\n\033[93;1m[*] " << msg << "\033[0m" << endl;
}

void pausar() {
  cout << "\nPresione una tecla para continuar...";
  system("pause >nul");
  cout << endl;
}

void limpiarPantalla() {
  system("cls");
}

void limpiarBuffer() {
  if (cin.fail()) {
    cin.clear();
  }
  cin.ignore(10000, '\n');
}

string obtenerFechaActual() {
  time_t now = time(nullptr);
  tm *ltm = localtime(&now);
  char buf[32];
  if (ltm) {
    sprintf(buf, "%02d/%02d/%04d %02d:%02d", ltm->tm_mday, 1 + ltm->tm_mon,
            1900 + ltm->tm_year, ltm->tm_hour, ltm->tm_min);
    return string(buf);
  }
  return "Fecha no disponible";
}

// Interfaz IImprimible
class IImprimible {
public:
  virtual void imprimirInfo() const = 0;
  virtual ~IImprimible() = default;
};

// Clase abstracta Usuario
class Usuario : public IImprimible {
protected:
  string id_usuario;
  string nombre;
  string email;
  string contrasena;

public:
  // Constructores y destructor
  Usuario() {
    id_usuario = "U000";
    nombre = "Sin nombre";
    email = "correo@ejemplo.com";
    contrasena = "1234";
  }

  Usuario(string id, string nom, string em, string pass) {
    id_usuario = id;
    nombre = nom;
    email = em;
    contrasena = pass;
  }

  virtual ~Usuario() = default;

  // Getters y setters
  string getIdUsuario() const { return id_usuario; }
  void setIdUsuario(const string &id) { id_usuario = id; }

  string getNombre() const { return nombre; }
  void setNombre(const string &nom) { nombre = nom; }

  string getEmail() const { return email; }
  void setEmail(const string &em) { email = em; }

  string getContrasena() const { return contrasena; }
  void setContrasena(const string &pass) { contrasena = pass; }

  // Metodos virtuales puros
  virtual string getRol() const = 0;
  virtual void imprimirInfo() const override = 0;
};

// Clase Cliente (hereda de Usuario)
class Cliente : public Usuario {
private:
  string telefono;
  string direccion;

public:
  // Constructores
  Cliente() : Usuario() {
    telefono = "000000000";
    direccion = "Sin direccion";
  }

  Cliente(string id, string nom, string em, string tel, string dir,
          string pass = "1234")
      : Usuario(id, nom, em, pass) {
    telefono = tel;
    direccion = dir;
  }

  // Getters y setters propios
  string getTelefono() const { return telefono; }
  void setTelefono(const string &tel) { telefono = tel; }

  string getDireccion() const { return direccion; }
  void setDireccion(const string &dir) { direccion = dir; }

  // Metodos polimorficos
  string getRol() const override { return "Cliente"; }

  void imprimirInfo() const override {
    cout << "  \033[96;1m[DATOS DEL CLIENTE]\033[0m" << endl;
    cout << "  ID Usuario:  \033[97m" << id_usuario << "\033[0m" << endl;
    cout << "  Nombre:      \033[97m" << nombre << "\033[0m" << endl;
    cout << "  Email:       \033[97m" << email << "\033[0m" << endl;
    cout << "  Telefono:    \033[97m" << telefono << "\033[0m" << endl;
    cout << "  Direccion:   \033[97m" << direccion << "\033[0m" << endl;
  }
};

// Clase Administrador (hereda de Usuario)
class Administrador : public Usuario {
private:
  string cargo;

public:
  // Constructores
  Administrador()
      : Usuario("ADM-01", "Administrador General", "admin@tienda.com",
                "admin123") {
    cargo = "Propietario / Gerente";
  }

  Administrador(string id, string nom, string em, string pass,
                string car = "Propietario / Gerente")
      : Usuario(id, nom, em, pass) {
    cargo = car;
  }

  // Getters y setters propios
  string getCargo() const { return cargo; }
  void setCargo(const string &car) { cargo = car; }

  // Metodos polimorficos
  string getRol() const override { return "Administrador"; }

  void imprimirInfo() const override {
    cout << "  \033[95;1m[DATOS DEL ADMINISTRADOR / DUENO]\033[0m" << endl;
    cout << "  Usuario ID:  \033[97m" << id_usuario << "\033[0m" << endl;
    cout << "  Nombre:      \033[97m" << nombre << "\033[0m" << endl;
    cout << "  Email:       \033[97m" << email << "\033[0m" << endl;
    cout << "  Cargo:       \033[97m" << cargo << "\033[0m" << endl;
  }
};

// Clase Autenticador
class Autenticador {
public:
  static bool validar(const Usuario *u, const string &usuarioOEmail,
                      const string &pass) {
    if (!u)
      return false;
    return (u->getIdUsuario() == usuarioOEmail ||
            u->getEmail() == usuarioOEmail) &&
           u->getContrasena() == pass;
  }
};

// Clase Producto
class Producto {
private:
  string codigo;
  string nombre;
  string marca;
  string modelo;
  string variante;
  double precio;
  int stock;

public:
  // Constructores
  Producto() {
    codigo = "P000";
    nombre = "Sin nombre";
    marca = "Generica";
    modelo = "Estandar";
    variante = "Normal";
    precio = 0.0;
    stock = 0;
  }

  Producto(string cod, string nom, string mar, string mod, string var,
           double pre, int stk) {
    codigo = cod;
    nombre = nom;
    marca = mar;
    modelo = mod;
    variante = var;
    setPrecio(pre);
    setStock(stk);
  }

  // Getters y setters con validacion
  string getCodigo() const { return codigo; }
  void setCodigo(const string &cod) { codigo = cod; }

  string getNombre() const { return nombre; }
  void setNombre(const string &nom) { nombre = nom; }

  string getMarca() const { return marca; }
  void setMarca(const string &mar) { marca = mar; }

  string getModelo() const { return modelo; }
  void setModelo(const string &mod) { modelo = mod; }

  string getVariante() const { return variante; }
  void setVariante(const string &var) { variante = var; }

  double getPrecio() const { return precio; }
  void setPrecio(double pre) { precio = (pre >= 0.0) ? pre : 0.0; }

  int getStock() const { return stock; }
  void setStock(int stk) { stock = (stk >= 0) ? stk : 0; }

  // Metodo para imprimir producto
  void imprimirProducto() const {
    cout << "  Codigo:   \033[93m" << codigo << "\033[0m" << endl;
    cout << "  Nombre:   \033[97;1m" << nombre << " (" << marca << " " << modelo << ")\033[0m" << endl;
    cout << "  Detalle:  " << variante << endl;
    cout << "  Precio:   \033[92;1mS/. " << fixed << setprecision(2) << precio << "\033[0m" << endl;
    cout << "  Stock:    \033[96m" << stock << " unidades disponibles\033[0m" << endl;
  }
};

// Clase abstracta ComprobantePago
class ComprobantePago : public IImprimible {
protected:
  string numero_comprobante;
  string fecha;
  string cliente_nombre;
  double subtotal;
  double total;

public:
  // Constructores y destructor
  ComprobantePago() {
    numero_comprobante = "C000-0000";
    fecha = "01/01/2026";
    cliente_nombre = "Consumidor Final";
    subtotal = 0.0;
    total = 0.0;
  }

  ComprobantePago(string num, string fec, string cli, double sub, double tot) {
    numero_comprobante = num;
    fecha = fec;
    cliente_nombre = cli;
    subtotal = sub;
    total = tot;
  }

  virtual ~ComprobantePago() = default;

  // Getters y setters
  string getNumeroComprobante() const { return numero_comprobante; }
  void setNumeroComprobante(const string &num) { numero_comprobante = num; }

  string getFecha() const { return fecha; }
  void setFecha(const string &fec) { fecha = fec; }

  string getClienteNombre() const { return cliente_nombre; }
  void setClienteNombre(const string &cli) { cliente_nombre = cli; }

  double getSubtotal() const { return subtotal; }
  void setSubtotal(double sub) { subtotal = sub; }

  double getTotal() const { return total; }
  void setTotal(double tot) { total = tot; }

  // Metodos virtuales puros
  virtual string getTipo() const = 0;
  virtual void imprimirInfo() const override = 0;
};

// Clase Boleta (hereda de ComprobantePago)
class Boleta : public ComprobantePago {
private:
  string dni_cliente;

public:
  // Constructores
  Boleta() : ComprobantePago() { dni_cliente = "00000000"; }

  Boleta(string num, string fec, string cli, string dni, double sub, double tot)
      : ComprobantePago(num, fec, cli, sub, tot) {
    dni_cliente = dni;
  }

  // Getters y setters
  string getDniCliente() const { return dni_cliente; }
  void setDniCliente(const string &dni) { dni_cliente = dni; }

  // Metodos polimorficos
  string getTipo() const override { return "Boleta de Venta"; }

  void imprimirInfo() const override {
    cout << "\033[96m============================================================\033[0m" << endl;
    cout << "\033[97;1m                    BOLETA DE VENTA                         \033[0m" << endl;
    cout << "\033[96m============================================================\033[0m" << endl;
    cout << "  Numero:     \033[93m" << numero_comprobante << "\033[0m" << endl;
    cout << "  Fecha:      " << fecha << endl;
    cout << "  Cliente:    \033[97m" << cliente_nombre << "\033[0m" << endl;
    cout << "  DNI:        " << dni_cliente << endl;
    imprimirSeparador();
    cout << "  Subtotal:   S/. " << fixed << setprecision(2) << subtotal << endl;
    cout << "  \033[92;1mTOTAL:      S/. " << fixed << setprecision(2) << total << "\033[0m" << endl;
    cout << "\033[96m============================================================\033[0m" << endl;
  }
};

// Clase Factura (hereda de ComprobantePago)
class Factura : public ComprobantePago {
private:
  string ruc_cliente;
  string razon_social;
  double igv;

public:
  // Constructores
  Factura() : ComprobantePago() {
    ruc_cliente = "00000000000";
    razon_social = "Sin razon social";
    igv = 0.0;
  }

  Factura(string num, string fec, string cli, string ruc, string rs, double sub,
          double tot)
      : ComprobantePago(num, fec, cli, sub, tot) {
    ruc_cliente = ruc;
    razon_social = rs;
    igv = tot - sub;
  }

  // Getters y setters
  string getRucCliente() const { return ruc_cliente; }
  void setRucCliente(const string &ruc) { ruc_cliente = ruc; }

  string getRazonSocial() const { return razon_social; }
  void setRazonSocial(const string &rs) { razon_social = rs; }

  double getIgv() const { return igv; }

  // Metodos polimorficos
  string getTipo() const override { return "Factura Electronica"; }

  void imprimirInfo() const override {
    cout << "\033[96m============================================================\033[0m" << endl;
    cout << "\033[97;1m                  FACTURA ELECTRONICA                       \033[0m" << endl;
    cout << "\033[96m============================================================\033[0m" << endl;
    cout << "  Numero:       \033[93m" << numero_comprobante << "\033[0m" << endl;
    cout << "  Fecha:        " << fecha << endl;
    cout << "  Razon Social: \033[97m" << razon_social << "\033[0m" << endl;
    cout << "  RUC:          " << ruc_cliente << endl;
    cout << "  Contacto:     " << cliente_nombre << endl;
    imprimirSeparador();
    cout << "  Subtotal Op. Gravada: S/. " << fixed << setprecision(2) << subtotal << endl;
    cout << "  I.G.V. (18%):         S/. " << fixed << setprecision(2) << igv << endl;
    cout << "  \033[92;1mTOTAL A PAGAR:        S/. " << fixed << setprecision(2) << total << "\033[0m" << endl;
    cout << "\033[96m============================================================\033[0m" << endl;
  }
};

// Clase Venta
class Venta {
private:
  string codigo_venta;
  string fecha;
  string nombre_cliente;
  string nombre_producto;
  int cantidad;
  double subtotal;
  double total;
  string metodo_pago;

public:
  // Constructores
  Venta() {
    codigo_venta = "V-000";
    fecha = "01/01/2026";
    nombre_cliente = "General";
    nombre_producto = "Producto";
    cantidad = 1;
    subtotal = 0.0;
    total = 0.0;
    metodo_pago = "Efectivo";
  }

  Venta(string cod, string fec, string cli, string prod, int cant, double sub,
        double tot, string mp) {
    codigo_venta = cod;
    fecha = fec;
    nombre_cliente = cli;
    nombre_producto = prod;
    cantidad = cant;
    subtotal = sub;
    total = tot;
    metodo_pago = mp;
  }

  // Getters y setters
  string getCodigoVenta() const { return codigo_venta; }
  void setCodigoVenta(const string &c) { codigo_venta = c; }

  string getFecha() const { return fecha; }
  void setFecha(const string &f) { fecha = f; }

  string getNombreCliente() const { return nombre_cliente; }
  void setNombreCliente(const string &cli) { nombre_cliente = cli; }

  string getNombreProducto() const { return nombre_producto; }
  void setNombreProducto(const string &prod) { nombre_producto = prod; }

  int getCantidad() const { return cantidad; }
  void setCantidad(int cant) { cantidad = cant; }

  double getSubtotal() const { return subtotal; }
  void setSubtotal(double sub) { subtotal = sub; }

  double getTotal() const { return total; }
  void setTotal(double tot) { total = tot; }

  string getMetodoPago() const { return metodo_pago; }
  void setMetodoPago(const string &mp) { metodo_pago = mp; }

  // Metodo para imprimir venta
  void imprimirVenta() const {
    cout << "\033[90m  --------------------------------------------------\033[0m" << endl;
    cout << "  \033[96;1m             DETALLE DE LA VENTA: " << codigo_venta << "\033[0m" << endl;
    cout << "\033[90m  --------------------------------------------------\033[0m" << endl;
    cout << "    Fecha:        " << fecha << endl;
    cout << "    Cliente:      \033[97m" << nombre_cliente << "\033[0m" << endl;
    cout << "    Producto:     \033[97m" << nombre_producto << " (x" << cantidad << ")\033[0m" << endl;
    cout << "    Metodo Pago:  \033[93m" << metodo_pago << "\033[0m" << endl;
    cout << "    Subtotal:     S/. " << fixed << setprecision(2) << subtotal << endl;
    cout << "    \033[92;1mTotal Pagado: S/. " << fixed << setprecision(2) << total << "\033[0m" << endl;
    cout << "\033[90m  --------------------------------------------------\033[0m" << endl;
  }
};

// Clase Tienda
class Tienda : public IImprimible {
private:
  string nombre;
  string direccion;
  string ruc;
  string telefono;
  vector<Producto> catalogo_productos;

public:
  // Constructores
  Tienda() {
    nombre = "Mi Tienda";
    direccion = "Av. Principal 123";
    ruc = "20100000001";
    telefono = "01-4444444";
  }

  Tienda(string nom, string dir, string r = "20100000001",
         string tel = "01-4444444") {
    nombre = nom;
    direccion = dir;
    ruc = r;
    telefono = tel;
  }

  // Getters y setters
  string getNombre() const { return nombre; }
  void setNombre(const string &nom) { nombre = nom; }

  string getDireccion() const { return direccion; }
  void setDireccion(const string &dir) { direccion = dir; }

  string getRuc() const { return ruc; }
  void setRuc(const string &r) { ruc = r; }

  string getTelefono() const { return telefono; }
  void setTelefono(const string &tel) { telefono = tel; }

  // Metodos del catalogo de productos
  vector<Producto> &getCatalogo() { return catalogo_productos; }
  const vector<Producto> &getCatalogo() const { return catalogo_productos; }
  int getCantidadProductos() const { return (int)catalogo_productos.size(); }

  Producto *getProducto(int index) {
    if (index >= 0 && index < (int)catalogo_productos.size()) {
      return &catalogo_productos[index];
    }
    return nullptr;
  }

  void agregarProducto(const Producto &p) { catalogo_productos.push_back(p); }

  void mostrarProductos() const {
    cout << "\n\033[96m============================================================\033[0m" << endl;
    cout << "  \033[97;1m           CATALOGO DE PRODUCTOS - " << nombre << "\033[0m" << endl;
    cout << "\033[96m============================================================\033[0m" << endl;

    if (catalogo_productos.empty()) {
      cout << "  \033[93m[*] No hay productos registrados en la tienda actualmente.\033[0m" << endl;
      cout << "      El administrador puede agregar productos desde su panel." << endl;
      return;
    }

    for (size_t i = 0; i < catalogo_productos.size(); i++) {
      const Producto &p = catalogo_productos[i];
      cout << "  \033[93;1m[" << (i + 1) << "]\033[0m ";
      cout << "\033[97;1m" << p.getNombre() << " (" << p.getMarca() << " " << p.getModelo() << ")\033[0m" << endl;
      cout << "      Detalle: " << p.getVariante() << endl;
      cout << "      Precio:  \033[92;1mS/. " << fixed << setprecision(2) << p.getPrecio() << "\033[0m" << endl;
      cout << "      Stock:   \033[96m" << p.getStock() << " unidades disponibles\033[0m" << endl;
      cout << "\033[90m  ----------------------------------------------------------\033[0m" << endl;
    }
  }

  // Metodos polimorficos
  void imprimirInfo() const override {
    cout << "\n\033[96m============================================================\033[0m" << endl;
    cout << "  \033[97;1mDATOS OFICIALES DE LA TIENDA\033[0m" << endl;
    cout << "\033[96m============================================================\033[0m" << endl;
    cout << "  Nombre Comercial:  \033[97m" << nombre << "\033[0m" << endl;
    cout << "  Direccion Fiscal:  \033[97m" << direccion << "\033[0m" << endl;
    cout << "  RUC del Negocio:   \033[97m" << ruc << "\033[0m" << endl;
    cout << "  Telefono Contacto: \033[97m" << telefono << "\033[0m" << endl;
    cout << "  Total de Items:    \033[93m" << catalogo_productos.size() << " productos en inventario\033[0m" << endl;
    cout << "\033[96m============================================================\033[0m" << endl;
  }
};

// Persistencia en archivos
const string DIR_DATOS = "datos/";

// Asegura la existencia de la carpeta 'datos' en disco
void asegurarDirectorioDatos() {
  _mkdir("datos");
}

static string trimStr(const string &s) {
  size_t a = s.find_first_not_of(" \t\r\n");
  if (a == string::npos)
    return "";
  size_t b = s.find_last_not_of(" \t\r\n");
  return s.substr(a, b - a + 1);
}

static vector<string> splitPipe(const string &linea) {
  vector<string> t;
  char delim = (linea.find('|') != string::npos) ? '|' : ';';
  stringstream ss(linea);
  string tok;
  while (getline(ss, tok, delim))
    t.push_back(trimStr(tok));
  return t;
}

// Persistencia de configuracion
void guardarConfig(const string &nom, const string &dir, const string &ruc,
                   const string &tel, const string &user, const string &pass,
                   int cnt) {
  asegurarDirectorioDatos();
  ofstream f(DIR_DATOS + "config.txt");
  if (f.is_open()) {
    f << "# FORMATO: NOMBRE_TIENDA|DIRECCION|RUC|TELEFONO|ADMIN_USER|ADMIN_PASS|CONTADOR_VENTAS\n";
    f << nom << "|" << dir << "|" << ruc << "|" << tel << "|" << user << "|"
      << pass << "|" << cnt << "\n";
    f.close();
  }
}

bool cargarConfig(string &nom, string &dir, string &ruc, string &tel,
                  string &user, string &pass, int &cnt) {
  ifstream f(DIR_DATOS + "config.txt");
  if (!f.is_open())
    return false;
  string linea;
  while (getline(f, linea)) {
    string c = trimStr(linea);
    if (c.empty() || c[0] == '#')
      continue;
    vector<string> t = splitPipe(c);
    if (t.size() >= 6) {
      nom = t[0];
      dir = t[1];
      ruc = t[2];
      tel = t[3];
      user = t[4];
      pass = t[5];
      cnt = (t.size() >= 7 && !t[6].empty()) ? atoi(t[6].c_str()) : 0;
      f.close();
      return true;
    } else if (t.size() >= 4) {
      nom = t[0];
      dir = t[1];
      ruc = "20100000001";
      tel = "01-4444444";
      user = t[2];
      pass = t[3];
      cnt = (t.size() >= 5 && !t[4].empty()) ? atoi(t[4].c_str()) : 0;
      f.close();
      return true;
    }
  }
  f.close();
  return false;
}

// Persistencia de productos
void guardarProductos(const Tienda &tienda) {
  asegurarDirectorioDatos();
  ofstream f(DIR_DATOS + "productos.txt");
  if (f.is_open()) {
    f << "# FORMATO: CODIGO|NOMBRE|MARCA|MODELO|VARIANTE|PRECIO|STOCK\n";
    for (const Producto &p : tienda.getCatalogo()) {
      f << p.getCodigo() << "|" << p.getNombre() << "|" << p.getMarca() << "|"
        << p.getModelo() << "|" << p.getVariante() << "|" << fixed
        << setprecision(2) << p.getPrecio() << "|" << p.getStock() << "\n";
    }
    f.close();
  }
}

bool cargarProductos(Tienda &tienda) {
  ifstream f(DIR_DATOS + "productos.txt");
  if (!f.is_open())
    return false;
  tienda.getCatalogo().clear();
  string linea;
  while (getline(f, linea)) {
    string c = trimStr(linea);
    if (c.empty() || c[0] == '#')
      continue;
    vector<string> t = splitPipe(c);
    if (t.size() >= 2 && !t[0].empty()) {
      double pre = (t.size() > 5 && !t[5].empty()) ? atof(t[5].c_str()) : 0.0;
      int stk = (t.size() > 6 && !t[6].empty()) ? atoi(t[6].c_str()) : 0;
      tienda.agregarProducto(Producto(t[0], (t.size() > 1) ? t[1] : "Producto",
                                      (t.size() > 2) ? t[2] : "Generica",
                                      (t.size() > 3) ? t[3] : "Estandar",
                                      (t.size() > 4) ? t[4] : "Normal", pre,
                                      stk));
    }
  }
  f.close();
  return true;
}

// Persistencia de clientes
void guardarClientes(const vector<Cliente> &clientes) {
  asegurarDirectorioDatos();
  ofstream f(DIR_DATOS + "clientes.txt");
  if (f.is_open()) {
    f << "# FORMATO: ID|NOMBRE|EMAIL|TELEFONO|DIRECCION|CONTRASENA\n";
    for (const Cliente &c : clientes) {
      f << c.getIdUsuario() << "|" << c.getNombre() << "|" << c.getEmail()
        << "|" << c.getTelefono() << "|" << c.getDireccion() << "|"
        << c.getContrasena() << "\n";
    }
    f.close();
  }
}

bool cargarClientes(vector<Cliente> &clientes) {
  ifstream f(DIR_DATOS + "clientes.txt");
  if (!f.is_open())
    return false;
  clientes.clear();
  string linea;
  while (getline(f, linea)) {
    string c = trimStr(linea);
    if (c.empty() || c[0] == '#')
      continue;
    vector<string> t = splitPipe(c);
    if (t.size() >= 2 && !t[0].empty()) {
      clientes.push_back(
          Cliente(t[0], t[1], (t.size() > 2) ? t[2] : "correo@ejemplo.com",
                  (t.size() > 3) ? t[3] : "000000000",
                  (t.size() > 4) ? t[4] : "Sin direccion",
                  (t.size() > 5 && !t[5].empty()) ? t[5] : "1234"));
    }
  }
  f.close();
  return true;
}

// Persistencia de ventas y comprobantes
void registrarVentaArchivo(const Venta &v) {
  asegurarDirectorioDatos();
  ofstream f(DIR_DATOS + "ventas.txt", ios::app);
  if (f.is_open()) {
    f << v.getCodigoVenta() << "|" << v.getFecha() << "|"
      << v.getNombreCliente() << "|" << v.getNombreProducto() << "|"
      << v.getCantidad() << "|" << fixed << setprecision(2) << v.getSubtotal()
      << "|" << v.getTotal() << "|" << v.getMetodoPago() << "\n";
    f.close();
  }
}

void registrarComprobanteArchivo(const ComprobantePago *c) {
  if (!c)
    return;
  asegurarDirectorioDatos();
  ofstream f(DIR_DATOS + "comprobantes.txt", ios::app);
  if (f.is_open()) {
    f << c->getTipo() << "|" << c->getNumeroComprobante() << "|"
      << c->getFecha() << "|" << c->getClienteNombre() << "|" << fixed
      << setprecision(2) << c->getSubtotal() << "|" << c->getTotal() << "\n";
    f.close();
  }
}

bool cargarComprobantes(vector<ComprobantePago *> &comprobantes) {
  ifstream f(DIR_DATOS + "comprobantes.txt");
  if (!f.is_open())
    return false;
  string linea;
  while (getline(f, linea)) {
    string c = trimStr(linea);
    if (c.empty() || c[0] == '#')
      continue;
    vector<string> t = splitPipe(c);
    if (t.size() >= 6) {
      string tipo = t[0];
      string num = t[1];
      string fec = t[2];
      string cli = t[3];
      double sub = atof(t[4].c_str());
      double tot = atof(t[5].c_str());

      if (tipo == "Factura Electronica") {
        comprobantes.push_back(
            new Factura(num, fec, cli, "20555666777", cli + " SAC", sub, tot));
      } else {
        comprobantes.push_back(new Boleta(num, fec, cli, "70123456", sub, tot));
      }
    }
  }
  f.close();
  return true;
}

// Prototipos de funciones
void portalCliente(Tienda &tienda, Cliente &clienteActual,
                   vector<Venta> &historialVentas,
                   vector<ComprobantePago *> &historialComprobantes,
                   int &contadorVentas, const string &adminUser,
                   const string &adminPass);

void adminInventario(Tienda &tienda);
void adminClientes(const vector<Cliente> &listaClientes);
void adminReporteVentas(const vector<Venta> &historialVentas,
                        const vector<ComprobantePago *> &historialComprobantes);
void adminDatosTienda(Tienda &tienda, string &adminUser, string &adminPass,
                      int contadorVentas);
void panelDuenio(Tienda &tienda, string &adminUser, string &adminPass,
                 vector<Cliente> &listaClientes, vector<Venta> &historialVentas,
                 vector<ComprobantePago *> &historialComprobantes,
                 int contadorVentas);

void loginCliente(Tienda &tienda, vector<Cliente> &listaClientes,
                  vector<Venta> &historialVentas,
                  vector<ComprobantePago *> &historialComprobantes,
                  int &contadorVentas, const string &adminUser,
                  const string &adminPass);
void loginDuenio(Tienda &tienda, string &adminUser, string &adminPass,
                 vector<Cliente> &listaClientes, vector<Venta> &historialVentas,
                 vector<ComprobantePago *> &historialComprobantes,
                 int contadorVentas);

// Modulo del cliente
void portalCliente(Tienda &tienda, Cliente &clienteActual,
                   vector<Venta> &historialVentas,
                   vector<ComprobantePago *> &historialComprobantes,
                   int &contadorVentas, const string &adminUser,
                   const string &adminPass) {
  int opcion = 0;
  while (opcion != 4) {
    limpiarPantalla();
    imprimirTitulo("PORTAL DEL CLIENTE - " + tienda.getNombre());
    cout << "  Cliente actual: \033[97;1m" << clienteActual.getNombre() << "\033[0m" << endl;
    cout << "  Correo:         " << clienteActual.getEmail() << endl;
    imprimirSeparador();
    cout << "  \033[93m[1]\033[0m Ver catalogo de productos disponibles" << endl;
    cout << "  \033[93m[2]\033[0m Realizar una compra" << endl;
    cout << "  \033[93m[3]\033[0m Mi perfil" << endl;
    cout << "  \033[91m[4] Volver al menu inicial\033[0m" << endl;
    imprimirSeparador();
    cout << "  Seleccione una opcion: ";
    if (!(cin >> opcion)) {
      limpiarBuffer();
      continue;
    }

    switch (opcion) {
    case 1: {
      limpiarPantalla();
      tienda.mostrarProductos();
      pausar();
      break;
    }
    case 2: {
      limpiarPantalla();
      imprimirTitulo("REALIZAR UNA COMPRA");

      if (tienda.getCantidadProductos() == 0) {
        mensajeAlerta("No hay productos disponibles en el catalogo en este momento.");
        pausar();
        break;
      }

      tienda.mostrarProductos();

      int numProd = 0;
      cout << "\n  Ingrese el numero del producto a comprar: ";
      if (!(cin >> numProd) || numProd < 1 || numProd > tienda.getCantidadProductos()) {
        limpiarBuffer();
        mensajeError("Seleccion no valida.");
        pausar();
        break;
      }

      Producto *prod = tienda.getProducto(numProd - 1);
      if (prod->getStock() <= 0) {
        mensajeError("Lo sentimos, este producto esta agotado por el momento.");
        pausar();
        break;
      }

      int cantidad = 0;
      cout << "  Stock disponible: \033[96m" << prod->getStock() << " unidades\033[0m" << endl;
      cout << "  Ingrese cantidad a comprar: ";
      if (!(cin >> cantidad) || cantidad <= 0) {
        limpiarBuffer();
        mensajeError("Cantidad ingresada no valida.");
        pausar();
        break;
      }

      if (cantidad > prod->getStock()) {
        mensajeError("Stock insuficiente. Solo quedan " + to_string(prod->getStock()) + " unidades.");
        pausar();
        break;
      }

      double subtotal = prod->getPrecio() * cantidad;
      cout << "\n  Subtotal de la compra: \033[92;1mS/. " << fixed << setprecision(2) << subtotal << "\033[0m" << endl;

      // Metodo de pago
      cout << "\n  Seleccione metodo de pago:" << endl;
      cout << "  \033[93m[1]\033[0m Efectivo" << endl;
      cout << "  \033[93m[2]\033[0m \033[95;1mYape\033[0m" << endl;
      cout << "  Opcion (1-2): ";
      int opPago = 1;
      cin >> opPago;
      string metodoPagoStr = (opPago == 2) ? "Yape" : "Efectivo";

      // Tipo de comprobante
      cout << "\n  Tipo de comprobante requerido:" << endl;
      cout << "  \033[93m[1]\033[0m Boleta de Venta (Con DNI)" << endl;
      cout << "  \033[93m[2]\033[0m Factura Electronica (Con RUC y Razon Social)" << endl;
      cout << "  Opcion: ";
      int opComp = 1;
      cin >> opComp;

      contadorVentas++;
      string fechaActual = obtenerFechaActual();
      string codVenta = "V-" + to_string(1000 + contadorVentas);

      ComprobantePago *nuevoComprobante = nullptr;

      if (opComp == 2) {
        cin.ignore();
        string rucCliente, rsCliente;
        cout << "  Ingrese RUC (11 digitos): ";
        getline(cin, rucCliente);
        cout << "  Ingrese Razon Social:     ";
        getline(cin, rsCliente);
        if (rucCliente.empty())
          rucCliente = "20000000001";
        if (rsCliente.empty())
          rsCliente = clienteActual.getNombre() + " SAC";

        string numFactura = "F001-" + to_string(1000 + contadorVentas);
        double subOpGravada = subtotal / 1.18;
        nuevoComprobante =
            new Factura(numFactura, fechaActual, clienteActual.getNombre(),
                        rucCliente, rsCliente, subOpGravada, subtotal);
      } else {
        cin.ignore();
        string dniCliente;
        cout << "  Ingrese DNI del cliente (8 digitos): ";
        getline(cin, dniCliente);
        if (dniCliente.empty())
          dniCliente = "00000000";

        string numBoleta = "B001-" + to_string(1000 + contadorVentas);
        nuevoComprobante =
            new Boleta(numBoleta, fechaActual, clienteActual.getNombre(),
                       dniCliente, subtotal, subtotal);
      }

      // Actualizar stock
      prod->setStock(prod->getStock() - cantidad);

      // Crear objeto venta
      Venta nuevaVenta(codVenta, fechaActual, clienteActual.getNombre(),
                       prod->getNombre(), cantidad, subtotal, subtotal,
                       metodoPagoStr);
      historialVentas.push_back(nuevaVenta);
      historialComprobantes.push_back(nuevoComprobante);

      // Guardar en archivos
      guardarProductos(tienda);
      registrarVentaArchivo(nuevaVenta);
      registrarComprobanteArchivo(nuevoComprobante);
      guardarConfig(tienda.getNombre(), tienda.getDireccion(), tienda.getRuc(),
                    tienda.getTelefono(), adminUser, adminPass, contadorVentas);

      // Mostrar resumen de la transaccion
      limpiarPantalla();
      imprimirTitulo("¡COMPRA REALIZADA CON EXITO!");
      cout << endl;
      nuevaVenta.imprimirVenta();
      cout << endl;

      // Imprimir comprobante
      nuevoComprobante->imprimirInfo();

      pausar();
      break;
    }
    case 3: {
      limpiarPantalla();
      imprimirTitulo("MI PERFIL DE USUARIO");

      // Imprimir informacion del usuario
      const Usuario *usuarioPtr = &clienteActual;
      usuarioPtr->imprimirInfo();

      imprimirSeparador();
      cout << "  ¿Desea actualizar sus datos de contacto?" << endl;
      cout << "  \033[93m[1]\033[0m Cambiar telefono" << endl;
      cout << "  \033[93m[2]\033[0m Cambiar direccion" << endl;
      cout << "  \033[91m[3] Volver atras\033[0m" << endl;
      cout << "  Opcion: ";
      int subOpc = 0;
      cin >> subOpc;
      cin.ignore();

      if (subOpc == 1) {
        string nuevoTel;
        cout << "  Nuevo telefono: ";
        getline(cin, nuevoTel);
        clienteActual.setTelefono(nuevoTel);
        mensajeExito("Telefono actualizado con exito.");
        pausar();
      } else if (subOpc == 2) {
        string nuevaDir;
        cout << "  Nueva direccion: ";
        getline(cin, nuevaDir);
        clienteActual.setDireccion(nuevaDir);
        mensajeExito("Direccion actualizada con exito.");
        pausar();
      }
      break;
    }
    case 4:
      break;
    default:
      mensajeError("Opcion no valida.");
      pausar();
    }
  }
}

// Modulo del administrador: inventario
void adminInventario(Tienda &tienda) {
  int opcion = 0;
  while (opcion != 4) {
    limpiarPantalla();
    imprimirTitulo("GESTION DE INVENTARIO - " + tienda.getNombre());
    cout << "  \033[93m[1]\033[0m Ver catalogo e inventario completo" << endl;
    cout << "  \033[93m[2]\033[0m Registrar nuevo producto" << endl;
    cout << "  \033[93m[3]\033[0m Modificar precio o reabastecer stock" << endl;
    cout << "  \033[91m[4] Volver al panel principal\033[0m" << endl;
    imprimirSeparador();
    cout << "  Seleccione una opcion: ";
    if (!(cin >> opcion)) {
      limpiarBuffer();
      continue;
    }

    switch (opcion) {
    case 1: {
      limpiarPantalla();
      tienda.mostrarProductos();
      pausar();
      break;
    }
    case 2: {
      limpiarPantalla();
      imprimirTitulo("REGISTRAR NUEVO PRODUCTO");
      cin.ignore();
      string cod, nom, mar, mod, var;
      double pre = 0.0;
      int stk = 0;

      cout << "  Codigo de producto: ";
      getline(cin, cod);
      cout << "  Nombre comercial:   ";
      getline(cin, nom);
      cout << "  Marca:              ";
      getline(cin, mar);
      cout << "  Modelo:             ";
      getline(cin, mod);
      cout << "  Detalle / Variante: ";
      getline(cin, var);
      cout << "  Precio de venta S/: ";
      cin >> pre;
      cout << "  Stock inicial:      ";
      cin >> stk;

      if (cod.empty())
        cod = "P00" + to_string(tienda.getCantidadProductos() + 1);
      if (nom.empty())
        nom = "Producto General";

      Producto nuevoProd(cod, nom, mar, mod, var, pre, stk);
      tienda.agregarProducto(nuevoProd);
      guardarProductos(tienda);

      mensajeExito("Producto '" + nom + "' agregado exitosamente al inventario.");
      pausar();
      break;
    }
    case 3: {
      limpiarPantalla();
      imprimirTitulo("MODIFICAR PRECIO / STOCK");
      tienda.mostrarProductos();

      if (tienda.getCantidadProductos() == 0) {
        pausar();
        break;
      }

      int idx = 0;
      cout << "\n  Seleccione el numero de producto a modificar: ";
      if (!(cin >> idx) || idx < 1 || idx > tienda.getCantidadProductos()) {
        limpiarBuffer();
        mensajeError("Seleccion no valida.");
        pausar();
        break;
      }

      Producto *p = tienda.getProducto(idx - 1);
      cout << "\n  Producto seleccionado: \033[97;1m" << p->getNombre() << "\033[0m" << endl;
      cout << "  \033[93m[1]\033[0m Modificar precio (Actual: \033[92;1mS/. " << fixed << setprecision(2)
           << p->getPrecio() << "\033[0m)" << endl;
      cout << "  \033[93m[2]\033[0m Reabastecer stock (Actual: \033[96m" << p->getStock() << " unidades\033[0m)" << endl;
      cout << "  Opcion: ";
      int opcMod = 0;
      cin >> opcMod;

      if (opcMod == 1) {
        double nuevoPre;
        cout << "  Nuevo precio S/.: ";
        cin >> nuevoPre;
        p->setPrecio(nuevoPre);
        guardarProductos(tienda);
        mensajeExito("Precio actualizado correctamente.");
      } else if (opcMod == 2) {
        int cantAdd;
        cout << "  Cantidad a ingresar al stock: ";
        cin >> cantAdd;
        p->setStock(p->getStock() + cantAdd);
        guardarProductos(tienda);
        mensajeExito("Stock actualizado. Nuevo stock: " + to_string(p->getStock()) + " unidades.");
      }
      pausar();
      break;
    }
    case 4:
      break;
    default:
      mensajeError("Opcion no valida.");
      pausar();
    }
  }
}

// Modulo del administrador: clientes
void adminClientes(const vector<Cliente> &listaClientes) {
  limpiarPantalla();
  imprimirTitulo("CLIENTES REGISTRADOS EN LA TIENDA");

  if (listaClientes.empty()) {
    mensajeAlerta("No hay clientes registrados en la tienda actualmente.");
  } else {
    for (size_t i = 0; i < listaClientes.size(); i++) {
      cout << "\n[Cliente #" << (i + 1) << "]" << endl;
      // Imprimir datos del cliente
      const Usuario *uPtr = &listaClientes[i];
      uPtr->imprimirInfo();
      cout << "  --------------------------------------------------" << endl;
    }
  }
  pausar();
}

// Modulo del administrador: reportes
void adminReporteVentas(
    const vector<Venta> &historialVentas,
    const vector<ComprobantePago *> &historialComprobantes) {
  limpiarPantalla();
  imprimirTitulo("REPORTE DE VENTAS Y COMPROBANTES");

  cout << "\n\033[96;1m>>> HISTORIAL DE TRANSACCIONES REGISTRADAS:\033[0m" << endl;

  if (historialVentas.empty()) {
    mensajeAlerta("Aun no se han registrado ventas.");
  } else {
    for (size_t i = 0; i < historialVentas.size(); i++) {
      cout << "\n\033[93m--- Transaccion #" << (i + 1) << " ---\033[0m" << endl;
      historialVentas[i].imprimirVenta();
    }
  }

  imprimirSeparador();
  cout << "\033[96;1m>>> COMPROBANTES EMITIDOS:\033[0m" << endl;

  if (historialComprobantes.empty()) {
    mensajeAlerta("No se han emitido comprobantes aun.");
  } else {
    for (size_t i = 0; i < historialComprobantes.size(); i++) {
      cout << "\n\033[93m[" << (i + 1) << "]\033[0m ";
      // Imprimir comprobante
      historialComprobantes[i]->imprimirInfo();
    }
  }

  pausar();
}

// Modulo del administrador: datos de la tienda
void adminDatosTienda(Tienda &tienda, string &adminUser, string &adminPass,
                      int contadorVentas) {
  limpiarPantalla();
  imprimirTitulo("DATOS DE LA TIENDA Y ADMINISTRACION");
  tienda.imprimirInfo();

  cout << "\n  ¿Desea modificar los datos del negocio?" << endl;
  cout << "  \033[93m[1]\033[0m Cambiar nombre comercial" << endl;
  cout << "  \033[93m[2]\033[0m Cambiar direccion fiscal" << endl;
  cout << "  \033[93m[3]\033[0m Cambiar RUC y Telefono" << endl;
  cout << "  \033[93m[4]\033[0m Cambiar credenciales de Administrador" << endl;
  cout << "  \033[91m[5] Regresar\033[0m" << endl;
  cout << "  Opcion: ";
  int opc = 0;
  if (!(cin >> opc)) {
    limpiarBuffer();
    return;
  }

  cin.ignore();
  if (opc == 1) {
    string nuevoNom;
    cout << "  Nuevo nombre de la tienda: ";
    getline(cin, nuevoNom);
    if (!nuevoNom.empty())
      tienda.setNombre(nuevoNom);
    guardarConfig(tienda.getNombre(), tienda.getDireccion(), tienda.getRuc(),
                  tienda.getTelefono(), adminUser, adminPass, contadorVentas);
    mensajeExito("Nombre de la tienda actualizado.");
    pausar();
  } else if (opc == 2) {
    string nuevaDir;
    cout << "  Nueva direccion fiscal: ";
    getline(cin, nuevaDir);
    if (!nuevaDir.empty())
      tienda.setDireccion(nuevaDir);
    guardarConfig(tienda.getNombre(), tienda.getDireccion(), tienda.getRuc(),
                  tienda.getTelefono(), adminUser, adminPass, contadorVentas);
    mensajeExito("Direccion actualizada.");
    pausar();
  } else if (opc == 3) {
    string nuevoRuc, nuevoTel;
    cout << "  Nuevo RUC:      ";
    getline(cin, nuevoRuc);
    cout << "  Nuevo Telefono: ";
    getline(cin, nuevoTel);
    if (!nuevoRuc.empty())
      tienda.setRuc(nuevoRuc);
    if (!nuevoTel.empty())
      tienda.setTelefono(nuevoTel);
    guardarConfig(tienda.getNombre(), tienda.getDireccion(), tienda.getRuc(),
                  tienda.getTelefono(), adminUser, adminPass, contadorVentas);
    mensajeExito("RUC y Telefono actualizados.");
    pausar();
  } else if (opc == 4) {
    string nuevoUser, nuevaPass;
    cout << "  Nuevo usuario administrador: ";
    getline(cin, nuevoUser);
    cout << "  Nueva clave de seguridad:    ";
    getline(cin, nuevaPass);
    if (!nuevoUser.empty())
      adminUser = nuevoUser;
    if (!nuevaPass.empty())
      adminPass = nuevaPass;
    guardarConfig(tienda.getNombre(), tienda.getDireccion(), tienda.getRuc(),
                  tienda.getTelefono(), adminUser, adminPass, contadorVentas);
    mensajeExito("Credenciales administrativas actualizadas.");
    pausar();
  }
}

// Panel principal del administrador
void panelDuenio(Tienda &tienda, string &adminUser, string &adminPass,
                 vector<Cliente> &listaClientes, vector<Venta> &historialVentas,
                 vector<ComprobantePago *> &historialComprobantes,
                 int contadorVentas) {
  int opcion = 0;
  while (opcion != 5) {
    limpiarPantalla();
    imprimirTitulo("PANEL DE CONTROL - " + tienda.getNombre());
    cout << "  \033[93m[1]\033[0m Inventario de Productos (Ver, agregar, cambiar precio)" << endl;
    cout << "  \033[93m[2]\033[0m Clientes Registrados en la Tienda" << endl;
    cout << "  \033[93m[3]\033[0m Reporte Contable (Ventas y Comprobantes)" << endl;
    cout << "  \033[93m[4]\033[0m Configuracion de la Tienda y Acceso Admin" << endl;
    cout << "  \033[91m[5] Cerrar sesion de administrador\033[0m" << endl;
    imprimirSeparador();
    cout << "  Seleccione una opcion: ";
    if (!(cin >> opcion)) {
      limpiarBuffer();
      continue;
    }

    switch (opcion) {
    case 1:
      adminInventario(tienda);
      break;
    case 2:
      adminClientes(listaClientes);
      break;
    case 3:
      adminReporteVentas(historialVentas, historialComprobantes);
      break;
    case 4:
      adminDatosTienda(tienda, adminUser, adminPass, contadorVentas);
      break;
    case 5:
      break;
    default:
      mensajeError("Opcion no valida.");
      pausar();
    }
  }
}

// Login y registro de clientes
void loginCliente(Tienda &tienda, vector<Cliente> &listaClientes,
                  vector<Venta> &historialVentas,
                  vector<ComprobantePago *> &historialComprobantes,
                  int &contadorVentas, const string &adminUser,
                  const string &adminPass) {
  int opc = 0;
  while (opc != 3) {
    limpiarPantalla();
    imprimirTitulo("ACCESO DE CLIENTE - " + tienda.getNombre());
    cout << "  \033[93m[1]\033[0m Iniciar sesion (Cliente ya registrado)" << endl;
    cout << "  \033[93m[2]\033[0m Registrarse como nuevo cliente" << endl;
    cout << "  \033[91m[3] Volver al menu principal\033[0m" << endl;
    imprimirSeparador();
    cout << "  Seleccione una opcion: ";
    if (!(cin >> opc)) {
      limpiarBuffer();
      continue;
    }

    if (opc == 1) {
      limpiarPantalla();
      imprimirTitulo("INICIAR SESION CLIENTE");
      cin.ignore();
      string usuarioOEmail, pass;
      cout << "  Correo o ID de usuario: ";
      getline(cin, usuarioOEmail);
      cout << "  Contrasena:             ";
      getline(cin, pass);

      bool autenticado = false;
      int clienteIndex = -1;

      for (size_t i = 0; i < listaClientes.size(); i++) {
        if (Autenticador::validar(&listaClientes[i], usuarioOEmail, pass)) {
          autenticado = true;
          clienteIndex = (int)i;
          break;
        }
      }

      if (autenticado) {
        mensajeExito("¡Bienvenido/a de vuelta, " +
                     listaClientes[clienteIndex].getNombre() + "!");
        pausar();
        portalCliente(tienda, listaClientes[clienteIndex], historialVentas,
                      historialComprobantes, contadorVentas, adminUser,
                      adminPass);
        guardarClientes(listaClientes);
        break;
      } else {
        mensajeError("Error: Correo/ID o contrasena incorrectos.");
        pausar();
      }
    } else if (opc == 2) {
      limpiarPantalla();
      imprimirTitulo("REGISTRO DE NUEVO CLIENTE");
      cin.ignore();
      string id = "CLI-" + to_string(100 + listaClientes.size() + 1);
      string nom, em, pass, tel, dir;

      cout << "  ID asignado por el sistema: \033[93m" << id << "\033[0m" << endl;
      cout << "  Nombre completo:       ";
      getline(cin, nom);
      cout << "  Correo electronico:    ";
      getline(cin, em);
      cout << "  Contrasena de acceso:  ";
      getline(cin, pass);
      cout << "  Telefono de contacto:  ";
      getline(cin, tel);
      cout << "  Direccion de entrega:  ";
      getline(cin, dir);

      if (nom.empty())
        nom = "Cliente " + id;
      if (em.empty())
        em = id + "@correo.com";
      if (pass.empty())
        pass = "1234";

      Cliente nuevoCli(id, nom, em, tel, dir, pass);
      listaClientes.push_back(nuevoCli);
      guardarClientes(listaClientes);

      mensajeExito("¡Cuenta creada exitosamente! Bienvenido, " + nom + ".");
      pausar();

      portalCliente(tienda, listaClientes.back(), historialVentas,
                    historialComprobantes, contadorVentas, adminUser,
                    adminPass);
      guardarClientes(listaClientes);
      break;
    }
  }
}

// Login del administrador
void loginDuenio(Tienda &tienda, string &adminUser, string &adminPass,
                 vector<Cliente> &listaClientes, vector<Venta> &historialVentas,
                 vector<ComprobantePago *> &historialComprobantes,
                 int contadorVentas) {
  limpiarPantalla();
  imprimirTitulo("ACCESO ADMINISTRATIVO - DUENO DEL NEGOCIO");
  cout << "  Acceso restringido para el propietario y gerencia." << endl;
  imprimirSeparador();

  cin.ignore();
  string userAdmin, passAdmin;
  cout << "  Usuario Administrador: ";
  getline(cin, userAdmin);
  cout << "  Clave de Seguridad:    ";
  getline(cin, passAdmin);

  if (userAdmin == adminUser && passAdmin == adminPass) {
    mensajeExito("Acceso autorizado. Bienvenido al panel de administracion.");
    pausar();
    panelDuenio(tienda, adminUser, adminPass, listaClientes, historialVentas,
                historialComprobantes, contadorVentas);
  } else {
    mensajeError("ERROR: Usuario o clave administrativa incorrectos.");
    pausar();
  }
}

// Funcion principal main
int main() {
  // Activar soporte de colores ANSI en la terminal
  activarTerminal();

  // Variables principales
  Tienda miTienda;
  string adminUser = "admin";
  string adminPass = "admin123";
  int contadorVentas = 0;

  vector<Cliente> listaClientes;
  vector<Venta> historialVentas;
  vector<ComprobantePago *> historialComprobantes;

  // Cargar configuracion o iniciar datos
  asegurarDirectorioDatos();

  string nomT, dirT, rucT, telT;
  bool hayConfiguracion = cargarConfig(nomT, dirT, rucT, telT, adminUser,
                                       adminPass, contadorVentas);

  if (hayConfiguracion) {
    miTienda = Tienda(nomT, dirT, rucT, telT);
    cargarProductos(miTienda);
    cargarClientes(listaClientes);
    cargarComprobantes(historialComprobantes);
  } else {
    // Datos iniciales si no existe configuracion
    limpiarPantalla();
    imprimirTitulo("CONFIGURACION INICIAL - REGISTRO DE SU TIENDA");
    cout << "  No se detecto configuracion previa en la carpeta datos/." << endl;
    cout << "  Por favor ingrese los datos de su negocio para comenzar:" << endl;
    cout << endl;
    imprimirSeparador();

    cout << "  Nombre de la tienda:         ";
    getline(cin, nomT);
    cout << "  Direccion del local:         ";
    getline(cin, dirT);
    cout << "  RUC de la empresa:           ";
    getline(cin, rucT);
    cout << "  Telefono de contacto:        ";
    getline(cin, telT);

    if (nomT.empty())
      nomT = "Mi Tienda Comercial";
    if (dirT.empty())
      dirT = "Av. Principal 123";
    if (rucT.empty())
      rucT = "20123456789";
    if (telT.empty())
      telT = "01-5555555";

    cout << endl;
    cout << "  --- CUENTA DE ADMINISTRADOR / PROPIETARIO ---" << endl;
    cout << "  Nombre de usuario admin:     ";
    getline(cin, adminUser);
    cout << "  Clave de acceso admin:       ";
    getline(cin, adminPass);

    if (adminUser.empty())
      adminUser = "admin";
    if (adminPass.empty())
      adminPass = "admin123";

    miTienda = Tienda(nomT, dirT, rucT, telT);

    // Guardar configuracion inicial
    guardarConfig(nomT, dirT, rucT, telT, adminUser, adminPass, contadorVentas);
    guardarProductos(miTienda);
    guardarClientes(listaClientes);

    mensajeExito("¡Tienda '" + nomT + "' configurada y guardada en datos/config.txt!");
    pausar();
  }

  // Menu principal
  int rol = 0;
  while (rol != 3) {
    limpiarPantalla();
    imprimirTitulo("BIENVENIDO A " + miTienda.getNombre());
    cout << "  Sistema Comercial Orientado a Objetos (C++)" << endl;
    cout << endl;
    cout << "  ¿Como desea ingresar al sistema?" << endl;
    cout << endl;
    cout << "  \033[93m[1]\033[0m Acceso Cliente             (Login o Registro nuevo)" << endl;
    cout << "  \033[93m[2]\033[0m Acceso Dueno/Administrador (Gestion del Negocio)" << endl;
    cout << "  \033[91m[3] Salir del sistema\033[0m" << endl;
    cout << endl;
    imprimirSeparador();
    cout << "  Seleccione su opcion (1-3): ";
    if (!(cin >> rol)) {
      limpiarBuffer();
      continue;
    }

    switch (rol) {
    case 1:
      loginCliente(miTienda, listaClientes, historialVentas,
                   historialComprobantes, contadorVentas, adminUser, adminPass);
      break;
    case 2:
      loginDuenio(miTienda, adminUser, adminPass, listaClientes,
                  historialVentas, historialComprobantes, contadorVentas);
      break;
    case 3:
      // Guardar datos al salir
      guardarConfig(miTienda.getNombre(), miTienda.getDireccion(),
                    miTienda.getRuc(), miTienda.getTelefono(), adminUser,
                    adminPass, contadorVentas);
      guardarProductos(miTienda);
      guardarClientes(listaClientes);
      cout << "\n\033[92;1m¡Gracias por visitar " << miTienda.getNombre() << "!\033[0m\n" << endl;
      break;
    default:
      mensajeError("Opcion incorrecta. Por favor elija 1, 2 o 3.");
      pausar();
    }
  }

  // Liberar memoria dinamica
  for (size_t i = 0; i < historialComprobantes.size(); i++) {
    delete historialComprobantes[i];
  }

  return 0;
}
