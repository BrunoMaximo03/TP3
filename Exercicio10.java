import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;
import java.util.Locale;
import java.io.*;
import java.util.Scanner;

//class show
class Show {
    // atributos
    private String show_id;
    private String type;
    private String title;
    private String director;
    private String[] cast;
    private String country;
    private Date date_added;
    private int release_year;
    private String rating;
    private String duration;
    private String[] listed_in;

    // construtor
    public Show() {
        this.show_id = this.type = this.title = this.director = this.country = this.rating = this.duration = "NaN";
        this.cast = new String[0];
        this.listed_in = new String[0];
        this.date_added = null;
        this.release_year = -1;
    }

    // método get e set
    public String getShow_id() {
        return show_id;
    }
    public void setShow_id(String show_id) {
        this.show_id = show_id;
    }

    public String getType() {
        return type;
    }
    public void setType(String type) {
        this.type = type;
    }

    public String getTitle() {
        return title;
    }
    public void setTitle(String title) {
        this.title = title;
    }

    public String getDirector() {
        return director;
    }
    public void setDirector(String director) {
        this.director = director;
    }

    public String[] getCast() {
        return cast;
    }
    public void setCast(String[] cast) {
        this.cast = cast;
    }

    public String getCountry() {
        return country;
    }
    public void setCountry(String country) {
        this.country = country;
    }

    public Date getDate_added() {
        return date_added;
    }
    public void setDate_added(Date date_added) {
        this.date_added = date_added;
    }

    public int getRelease_year() {
        return release_year;
    }
    public void setRelease_year(int release_year) {
        this.release_year = release_year;
    }

    public String getRating() {
        return rating;
    }
    public void setRating(String rating) {
        this.rating = rating;
    }

    public String getDuration() {
        return duration;
    }
    public void setDuration(String duration) {
        this.duration = duration;
    }

    public String[] getListed_in() {
        return listed_in;
    }
    public void setListed_in(String[] listed_in) {
        this.listed_in = listed_in;
    }

    // método ler
    public void lerLinha(String linha) {
        try {
            String[] campos = linha.split(",(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)", -1);
            this.show_id = campos[0];
            this.type = campos[1];
            this.title = campos[2].replace("\"", "").trim();
            this.director = campos[3].isEmpty() ? "NaN" : campos[3].replace("\"", "").trim();
            this.cast = campos[4].isEmpty() ? new String[]{"NaN"} : campos[4].replace("\"", "").split(",\\s*");
            Arrays.sort(this.cast);
            this.country = campos[5].isEmpty() ? "NaN" : campos[5].trim().replace("\"", "");

            StringBuilder conteudoCampo = new StringBuilder();
            boolean entreAspas = false;
            int posicaoCampo = 0;

            for (int i = 0; i < linha.length(); i++) {
                char c = linha.charAt(i);

                if (c == '"') {
                    entreAspas = !entreAspas;
                } else if (c == ',' && !entreAspas) {
                    if (posicaoCampo == 6) {
                        break;
                    }
                    posicaoCampo++;
                    conteudoCampo.setLength(0);
                } else {
                    conteudoCampo.append(c);
                }
            }

            String valorData = conteudoCampo.toString().trim();
            try {
                SimpleDateFormat sdf = new SimpleDateFormat("MMMM d, yyyy", Locale.US);
                this.date_added = valorData.isEmpty() ? sdf.parse("March 1, 1900") : sdf.parse(valorData);
            } catch (java.text.ParseException e) {
                this.date_added = null;
            }

            this.release_year = campos[7].isEmpty() ? -1 : Integer.parseInt(campos[7].trim());
            this.rating = campos[8].isEmpty() ? "NaN" : campos[8].trim();
            this.duration = campos[9].isEmpty() ? "NaN" : campos[9].trim();
            this.listed_in = campos[10].isEmpty() ? new String[]{"NaN"} : campos[10].replace("\"", "").split(",\\s*");
            Arrays.sort(this.listed_in);
        } catch (Exception e) {
            System.out.println("Erro ao ler linha: " + e.getMessage());
        }
    }

    // método imprimir
    public void imprimir() {
        System.out.print("=> " + show_id + " ## " + title + " ## " + type + " ## " + director + " ## [");
        System.out.print(String.join(", ", cast));
        System.out.print("] ## " + country + " ## ");

        if (date_added != null) {
            SimpleDateFormat sdf = new SimpleDateFormat("MMMM d, yyyy", Locale.US);
            System.out.print(sdf.format(date_added));
        } else {
            System.out.print("NaN");
        }

        System.out.print(" ## " + release_year + " ## " + rating + " ## " + duration + " ## [");
        System.out.print(String.join(", ", listed_in));
        System.out.println("] ##");
    }

    public Show clone() {
        Show copia = new Show();
        copia.show_id = this.show_id;
        copia.type = this.type;
        copia.title = this.title;
        copia.director = this.director;
        copia.cast = this.cast.clone();
        copia.country = this.country;
        copia.date_added = this.date_added;
        copia.release_year = this.release_year;
        copia.rating = this.rating;
        copia.duration = this.duration;
        copia.listed_in = this.listed_in.clone();
        return copia;
    }
}

// Classe para celula dupla
class CelulaDupla {
    public int elemento;
    public CelulaDupla ant, prox;

    public CelulaDupla() {
        this(0);
    }

    public CelulaDupla(int elemento) {
        this.elemento = elemento;
        this.ant = this.prox = null;
    }
}

// Lista Duplamente Encadeada
class ListaDupla {
    private CelulaDupla primeiro;
    private CelulaDupla ultimo;

    // Construtor
    public ListaDupla() {
        primeiro = new CelulaDupla();
        ultimo = primeiro;
    }

    // inserir inicio
    public void inserirInicio(int x) {
        CelulaDupla tmp = new CelulaDupla(x);
        tmp.ant = primeiro;
        tmp.prox = primeiro.prox;
        primeiro.prox = tmp;
        if (primeiro == ultimo) {
            ultimo = tmp;
        } else {
            tmp.prox.ant = tmp;
        }
        tmp = null;
    }

    //inserir fim
    public void inserirFim(int x) {
        ultimo.prox = new CelulaDupla(x);
        ultimo.prox.ant = ultimo;
        ultimo = ultimo.prox;
    }

    // remover inicio
    public int removerInicio() throws Exception {
        if (primeiro == ultimo) {
            throw new Exception("Erro ao remover (vazia)!");
        }
        CelulaDupla tmp = primeiro;
        primeiro = primeiro.prox;
        int resp = primeiro.elemento;
        tmp.prox = primeiro.ant = null;
        tmp = null;
        return resp;
    }

    // remover fim
    public int removerFim() throws Exception {
        if (primeiro == ultimo) {
            throw new Exception("Erro ao remover (vazia)!");
        }
        int resp = ultimo.elemento;
        ultimo = ultimo.ant;
        ultimo.prox.ant = null;
        ultimo.prox = null;
        return resp;
    }

    // inserir em qualquer posição
    public void inserir(int x, int pos) throws Exception {
        int tamanho = tamanho();
        if (pos < 0 || pos > tamanho) {
            throw new Exception("Erro ao inserir posicao (" + pos + " / tamanho = " + tamanho + ") invalida!");
        } else if (pos == 0) {
            inserirInicio(x);
        } else if (pos == tamanho) {
            inserirFim(x);
        } else {
            CelulaDupla i = primeiro;
            for (int j = 0; j < pos; j++, i = i.prox) ;
            CelulaDupla tmp = new CelulaDupla(x);
            tmp.ant = i;
            tmp.prox = i.prox;
            tmp.ant.prox = tmp.prox.ant = tmp;
            tmp = i = null;
        }
    }

    // remover em qualquer posição
    public int remover(int pos) throws Exception {
        int resp;
        int tamanho = tamanho();
        if (primeiro == ultimo) {
            throw new Exception("Erro ao remover (vazia)!");
        } else if (pos < 0 || pos >= tamanho) {
            throw new Exception("Erro ao remover (posicao " + pos + " / " + tamanho + " invalida!");
        } else if (pos == 0) {
            resp = removerInicio();
        } else if (pos == tamanho - 1) {
            resp = removerFim();
        } else {
            CelulaDupla i = primeiro.prox;
            for (int j = 0; j < pos; j++, i = i.prox) ;
            i.ant.prox = i.prox;
            i.prox.ant = i.ant;
            resp = i.elemento;
            i.prox = i.ant = null;
            i = null;
        }
        return resp;
    }

    // mostrar
    public void mostrar() {
        System.out.print("[ ");
        for (CelulaDupla i = primeiro.prox; i != null; i = i.prox) {
            System.out.print(i.elemento + " ");
        }
        System.out.println("] ");
    }

    //mostrar inverso
    public void mostrarInverso() {
        System.out.print("[ ");
        for (CelulaDupla i = ultimo; i != primeiro; i = i.ant) {
            System.out.print(i.elemento + " ");
        }
        System.out.println("] ");
    }

    public boolean pesquisar(int x) {
        boolean resp = false;
        for (CelulaDupla i = primeiro.prox; i != null; i = i.prox) {
            if (i.elemento == x) {
                resp = true;
                i = ultimo;
            }
        }
        return resp;
    }

    public int tamanho() {
        int tamanho = 0;
        for (CelulaDupla i = primeiro; i != ultimo; i = i.prox, tamanho++) ;
        return tamanho;
    }

    // Obtem o elemento da posicao 
    public int get(int pos) {
        CelulaDupla i = primeiro.prox;
        for (int j = 0; j < pos; j++, i = i.prox) ;
        return i.elemento;
    }

    // Atualiza o elemento da posicao 
    public void set(int pos, int x) {
        CelulaDupla i = primeiro.prox;
        for (int j = 0; j < pos; j++, i = i.prox) ;
        i.elemento = x;
    }
}

// Main
public class Exercicio10 {
    static Show[] catalogo = new Show[10000];
    static int tamanhoCatalogo = 0;
    static long inicio;
    static int[] comparacoes = {0};
    static int[] movimentacoes = {0};

    public static void readCSV() {
        String caminho = "/tmp/disneyplus.csv";
        try {
            Scanner sc = new Scanner(new File(caminho));
            sc.nextLine();

            while (sc.hasNextLine()) {
                String linha = sc.nextLine();
                Show s = new Show();
                s.lerLinha(linha);
                catalogo[tamanhoCatalogo++] = s;
            }
            sc.close();
        } catch (IOException e) {
            System.out.println("Erro ao ler o arquivo: " + e.getMessage());
        }
    }

    public static boolean isFim(String str) {
        return str.equals("FIM");
    }

    // Implementação do QuickSort para lista duplamente encadeada
    public static void quickSortLista(ListaDupla lista, Show[] shows) {
        quickSortListaRec(lista, shows, 0, lista.tamanho() - 1);
    }

    public static void quickSortListaRec(ListaDupla lista, Show[] shows, int esq, int dir) {
        if (esq < dir) {
            int pivo = particionarLista(lista, shows, esq, dir);
            quickSortListaRec(lista, shows, esq, pivo - 1);
            quickSortListaRec(lista, shows, pivo + 1, dir);
        }
    }

    public static int particionarLista(ListaDupla lista, Show[] shows, int esq, int dir) {
        Show pivo = shows[lista.get(dir)];
        int i = esq - 1;
        for (int j = esq; j < dir; j++) {
            Show atual = shows[lista.get(j)];
            // Critério de ordenação por date_added, depois title
            if (atual.getDate_added().compareTo(pivo.getDate_added()) < 0 ||
                (atual.getDate_added().compareTo(pivo.getDate_added()) == 0 &&
                atual.getTitle().compareToIgnoreCase(pivo.getTitle()) < 0)) {
                i++;
                // swap na lista duplamente encadeada
                int idxI = lista.get(i);
                int idxJ = lista.get(j);
                lista.set(i, idxJ);
                lista.set(j, idxI);
                movimentacoes[0]++;
            }
            comparacoes[0]++;
        }
        // swap i+1, dir
        int idxI = lista.get(i + 1);
        int idxD = lista.get(dir);
        lista.set(i + 1, idxD);
        lista.set(dir, idxI);
        movimentacoes[0]++;
        return i + 1;
    }

    public static void main(String[] args) {
        inicio = System.nanoTime();

        readCSV();
        Scanner sc = new Scanner(System.in);

        ListaDupla lista = new ListaDupla();
        String entrada = sc.nextLine();
        Show[] resultados = new Show[10000];
        int tamanhoResultados = 0;

        // Adiciona índices dos selecionados à lista duplamente encadeada
        while (!isFim(entrada)) {
            for (int i = 0; i < tamanhoCatalogo; i++) {
                if (catalogo[i].getShow_id().equals(entrada)) {
                    lista.inserirFim(i);
                    resultados[tamanhoResultados++] = catalogo[i];
                    break;
                }
            }
            entrada = sc.nextLine();
        }

        // Ordena usando quicksort na lista duplamente encadeada
        quickSortLista(lista, catalogo);

        // Imprime todos os resultados ordenados 
        for (int i = 0; i < lista.tamanho(); i++) {
            Show show = catalogo[lista.get(i)];
            show.imprimir();
        }

        long fim = System.nanoTime();
        sc.close();

        double tempoExecucao = (fim - inicio) / 1e6;

        try {
            FileWriter fw = new FileWriter("matricula_quick.txt");
            fw.write("850847\t" + comparacoes[0] + "\t" + movimentacoes[0] + "\t" + tempoExecucao);
            fw.close();
        } catch (IOException e) {
            System.out.println("Erro ao gravar log: " + e.getMessage());
        }
    }
}