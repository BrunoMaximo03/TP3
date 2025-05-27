import java.io.*;
import java.text.SimpleDateFormat;
import java.util.*;

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

    public Show() { // contrutor padrão
        this.show_id = this.type = this.title = this.director = this.country = this.rating = this.duration = "NaN";
        this.cast = new String[0];
        this.listed_in = new String[0];
        this.date_added = null;
        this.release_year = -1;
    }

    // nos sets e nos gets e sets ja posso tratar o ID arrancando o "s" e
    // tranformando o id em int.

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

    public void readCSV(String linhaCSV) {
        try {
            List<String> camposSeparados = new ArrayList<>();
            StringBuilder bufferCampo = new StringBuilder();
            boolean dentroAspas = false;

            for (int i = 0; i < linhaCSV.length(); i++) {
                char caractere = linhaCSV.charAt(i);

                if (caractere == '"') {
                    dentroAspas = !dentroAspas;
                } else if (caractere == ',' && !dentroAspas) {
                    camposSeparados.add(bufferCampo.toString().trim());
                    bufferCampo.setLength(0);
                } else {
                    bufferCampo.append(caractere);
                }
            }

            camposSeparados.add(bufferCampo.toString().trim());

            String[] campos = camposSeparados.toArray(new String[0]);

            this.show_id = campos[0];
            this.type = campos[1];
            this.title = campos[2].replace("\"", "");
            this.director = campos[3].isEmpty() ? "NaN" : campos[3];
            this.cast = campos[4].isEmpty() ? new String[] { "NaN" } : campos[4].split(",\\s*");
            Arrays.sort(this.cast);
            this.country = campos[5].isEmpty() ? "NaN" : campos[5];

            try {
                SimpleDateFormat formatoData = new SimpleDateFormat("MMMM dd, yyyy", Locale.US);
                this.date_added = campos[6].isEmpty() ? null : formatoData.parse(campos[6]);
            } catch (Exception e) {
                this.date_added = null;
            }

            this.release_year = campos[7].isEmpty() ? -1 : Integer.parseInt(campos[7]);
            this.rating = campos[8].isEmpty() ? "NaN" : campos[8];
            this.duration = campos[9].isEmpty() ? "NaN" : campos[9];
            this.listed_in = campos[10].isEmpty() ? new String[] { "NaN" } : campos[10].split(",\\s*");
            Arrays.sort(this.listed_in);

        } catch (Exception e) {
            System.out.println("Erro ao ler linha: " + e.getMessage());
        }
    }

    public void imprimir() {
        System.out.print("=> " + show_id + " ## " + title + " ## " + type + " ## " + director + " ## [");
        System.out.print(String.join(", ", cast));
        System.out.print("] ## " + country + " ## ");

        if (date_added != null) {
            SimpleDateFormat Saida = new SimpleDateFormat("MMMM d, yyyy", Locale.US);
            System.out.print(Saida.format(date_added));
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

class ListaDupla {
    private CelulaDupla primeiro;
    private CelulaDupla ultimo;

    public Show get(int index) {
        CelulaDupla atual = primeiro.prox;
        for (int i = 0; i < index; i++) {
            atual = atual.prox;
        }
        return atual.elemento;
    }

    public void set(int index, Show value) {
        CelulaDupla atual = primeiro.prox;
        for (int i = 0; i < index; i++) {
            atual = atual.prox;
        }
        atual.elemento = value;
    }

    public void swap(int i, int j) {
        Show temp = get(i);
        set(i, get(j));
        set(j, temp);
    }

    public int getTamanho() {
        return tamanho();
    }

    /**
     * Construtor da classe que cria uma lista dupla sem elementos (somente no
     * cabeca).
     */
    public ListaDupla() {
        primeiro = new CelulaDupla();
        ultimo = primeiro;
    }

    /**
     * Insere um elemento na primeira posicao da lista.
     * 
     * @param x int elemento a ser inserido.
     */
    public void inserirInicio(Show x) {
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

    /**
     * Insere um elemento na ultima posicao da lista.
     * 
     * @param x int elemento a ser inserido.
     */
    public void inserirFim(Show x) {
        ultimo.prox = new CelulaDupla(x);
        ultimo.prox.ant = ultimo;
        ultimo = ultimo.prox;
    }

    /**
     * Remove um elemento da primeira posicao da lista.
     * 
     * @return resp int elemento a ser removido.
     * @throws Exception Se a lista nao contiver elementos.
     */
    public Show removerInicio() throws Exception {
        if (primeiro == ultimo) {
            throw new Exception("Erro ao remover (vazia)!");
        }

        CelulaDupla tmp = primeiro;
        primeiro = primeiro.prox;
        Show resp = primeiro.elemento;
        tmp.prox = primeiro.ant = null;
        tmp = null;
        return resp;
    }

    /**
     * Remove um elemento da ultima posicao da lista.
     * 
     * @return resp int elemento a ser removido.
     * @throws Exception Se a lista nao contiver elementos.
     */
    public Show removerFim() throws Exception {
        if (primeiro == ultimo) {
            throw new Exception("Erro ao remover (vazia)!");
        }
        Show resp = ultimo.elemento;
        ultimo = ultimo.ant;
        ultimo.prox.ant = null;
        ultimo.prox = null;
        return resp;
    }

    /**
     * Insere um elemento em uma posicao especifica considerando que o
     * primeiro elemento valido esta na posicao 0.
     * 
     * @param x   int elemento a ser inserido.
     * @param pos int posicao da insercao.
     * @throws Exception Se <code>posicao</code> invalida.
     */
    public void inserir(Show x, int pos) throws Exception {

        int tamanho = tamanho();

        if (pos < 0 || pos > tamanho) {
            throw new Exception("Erro ao inserir posicao (" + pos + " / tamanho = " + tamanho + ") invalida!");
        } else if (pos == 0) {
            inserirInicio(x);
        } else if (pos == tamanho) {
            inserirFim(x);
        } else {
            // Caminhar ate a posicao anterior a insercao
            CelulaDupla i = primeiro;
            for (int j = 0; j < pos; j++, i = i.prox)
                ;

            CelulaDupla tmp = new CelulaDupla(x);
            tmp.ant = i;
            tmp.prox = i.prox;
            tmp.ant.prox = tmp.prox.ant = tmp;
            tmp = i = null;
        }
    }

    /**
     * Remove um elemento de uma posicao especifica da lista
     * considerando que o primeiro elemento valido esta na posicao 0.
     * 
     * @param posicao Meio da remocao.
     * @return resp int elemento a ser removido.
     * @throws Exception Se <code>posicao</code> invalida.
     */
    public Show remover(int pos) throws Exception {
        Show resp;
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
            // Caminhar ate a posicao anterior a insercao
            CelulaDupla i = primeiro.prox;
            for (int j = 0; j < pos; j++, i = i.prox)
                ;

            i.ant.prox = i.prox;
            i.prox.ant = i.ant;
            resp = i.elemento;
            i.prox = i.ant = null;
            i = null;
        }

        return resp;
    }

    /**
     * Mostra os elementos da lista separados por espacos.
     */
    public void mostrar() {
        System.out.print("[ "); // Comeca a mostrar.
        for (CelulaDupla i = primeiro.prox; i != null; i = i.prox) {
            System.out.print(i.elemento + " ");
        }
        System.out.println("] "); // Termina de mostrar.
    }

    /**
     * Mostra os elementos da lista de forma invertida
     * e separados por espacos.
     */
    public void mostrarInverso() {
        System.out.print("[ ");
        for (CelulaDupla i = ultimo; i != primeiro; i = i.ant) {
            System.out.print(i.elemento + " ");
        }
        System.out.println("] "); // Termina de mostrar.
    }

    /**
     * Procura um elemento e retorna se ele existe.
     * 
     * @param x Elemento a pesquisar.
     * @return <code>true</code> se o elemento existir,
     *         <code>false</code> em caso contrario.
     */
    public boolean pesquisar(Show x) {
        boolean resp = false;
        for (CelulaDupla i = primeiro.prox; i != null; i = i.prox) {
            if (i.elemento == x) {
                resp = true;
                i = ultimo;
            }
        }
        return resp;
    }

    /**
     * Calcula e retorna o tamanho, em numero de elementos, da lista.
     * 
     * @return resp int tamanho
     */
    public int tamanho() {
        int tamanho = 0;
        for (CelulaDupla i = primeiro; i != ultimo; i = i.prox, tamanho++)
            ;
        return tamanho;
    }

    class CelulaDupla {
        public Show elemento;
        public CelulaDupla ant;
        public CelulaDupla prox;

        /**
         * Construtor da classe.
         */
        public CelulaDupla() {
            elemento = null;
            ant = prox = null;
        }

        /**
         * Construtor da classe.
         * 
         * @param elemento Show é inserido na celula.
         */
        public CelulaDupla(Show elemento) {
            this.elemento = elemento;
            this.ant = this.prox = null;
        }
    }
}

// classe main
public class Exercicio08 {

    static ListaDupla lista = new ListaDupla();
    static int tamanhoCatalogo = 0;
    static int[] comparacoes = { 0 };
    static int[] movimentacoes = { 0 };

    public static void quickSortLista(int esq, int dir) {
        if (esq < dir) {
            int pivo = particionarLista(esq, dir);
            quickSortLista(esq, pivo - 1);
            quickSortLista(pivo + 1, dir);
        }
    }

    // Método que particiona o array para o Quick Sort
    public static int particionarLista(int esq, int dir) {
        Show pivo = lista.get(dir);
        int i = esq - 1;

        for (int j = esq; j < dir; j++) {
            Show atual = lista.get(j);

            if (atual.getDate_added().compareTo(pivo.getDate_added()) < 0 ||
                    (atual.getDate_added().compareTo(pivo.getDate_added()) == 0 &&
                            atual.getTitle().compareToIgnoreCase(pivo.getTitle()) < 0)) {
                i++;
                lista.swap(i, j);
                movimentacoes[0]++;
            }
            comparacoes[0]++;
        }

        lista.swap(i + 1, dir);
        movimentacoes[0]++;
        return i + 1;
    }

    // faz um swap
    public static void swap(Show[] array, int i, int j) {
        Show temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }

    static List<Show> catalogo = new ArrayList<>();

    public static void preencherCatalogo() {
        String caminho = "/tmp/disneyplus.csv";
        try {
            BufferedReader br = new BufferedReader(new FileReader(caminho));
            br.readLine();

            String linha;
            while ((linha = br.readLine()) != null) {
                Show s = new Show();
                s.readCSV(linha);
                catalogo.add(s);
            }

            br.close();
        } catch (IOException e) {
            System.out.println("Erro ao ler arquivo: " + e.getMessage());
        }
    }

    public static boolean isFim(String str) {
        return str.equals("FIM");
    }

    // main principal
    public static void main(String[] args) {
        preencherCatalogo();

        Scanner sc = new Scanner(System.in);
        String entrada;

        // Leitura dos IDs e inserção na lista
        while (!(entrada = sc.nextLine()).equals("FIM")) {
            for (Show s : catalogo) {
                if (s.getShow_id().equals(entrada)) {
                    lista.inserirFim(s.clone());
                    break;
                }
            }
        }

        long inicio = System.currentTimeMillis();

        // Ordenação com QuickSort
        quickSortLista(0, lista.getTamanho() - 1);

        long fim = System.currentTimeMillis();

        // Impressão dos elementos ordenados
        for (int i = 0; i < lista.getTamanho(); i++) {
            lista.get(i).imprimir();
        }

        // Gravação do log
        try {
            BufferedWriter log = new BufferedWriter(new FileWriter("matricula_quicksort.txt"));
            log.write("850847\t" + comparacoes[0] + "\t" + movimentacoes[0] + "\t" + (fim - inicio) + "ms");
            log.close();
        } catch (IOException e) {
            System.out.println("Erro ao gravar log: " + e.getMessage());
        }

        sc.close();
    }
}
