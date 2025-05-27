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

class Pilha {
    private Celula topo;

    public Pilha() {
        topo = null;
    }

    public void inserir(Show x) {
		Celula tmp = new Celula(x);
		tmp.prox = topo;
		topo = tmp;
		tmp = null;
	}

    public Show remover() throws Exception {
		if (topo == null) {
			throw new Exception("Erro ao remover!");
		}
		Show resp = topo.elemento;
		Celula tmp = topo;
		topo = topo.prox;
		tmp.prox = null;
		tmp = null;
		return resp;
	}

    public int getTamanho() {
        int tamanho = 0;
        for(Celula i = topo; i != null; i = i.prox) {
            tamanho++;
        }
        return tamanho;
    }

    public void mostrar() {
		System.out.print("[ ");
		for (Celula i = topo; i != null; i = i.prox) {
			System.out.print(i.elemento + " ");
		}
		System.out.println("] ");
	}

	public void mostraPilha() {
		mostraPilha(topo);
	}

	private void mostraPilha(Celula i) {
		if (i != null) {
			mostraPilha(i.prox);
			System.out.println("" + i.elemento);
		}
	}


    class Celula {
        public Show elemento;
        public Celula prox;

        /**
         * Construtor da classe.
         */
        public Celula() {
            elemento = null;
            prox = null;
        }

        /**
         * Construtor da classe.
         * 
         * @param elemento Show é inserido na celula.
         */
        public Celula(Show elemento) {
            this.elemento = elemento;
            this.prox = null;
        }
    }
}

// classe main
public class Exercicio09 {
    
    static List<Show> catalogo = new ArrayList<>();

    public static void preencherCatalogo() {
        String caminho = "disneyplus.csv";
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
        Pilha pilha = new Pilha();
        Scanner scanner = new Scanner(System.in);
        String entrada;

        // Leitura dos IDs e inserção na lista
        while (!(entrada = scanner.nextLine()).equals("FIM")) {
            for (Show s : catalogo) {
                if (s.getShow_id().equals(entrada)) {
                    pilha.inserir(s.clone());
                    break;
                }
            }
        }

        System.out.println("Informe quantos serão removidos ou inseridos");
        int tamannhoManipulacaoPilha = scanner.nextInt();
        scanner.nextLine(); // Limpar o buffer do scanner

        while(tamannhoManipulacaoPilha-- > 0) {
            String operacao = scanner.nextLine();
            if (operacao.equals("R")) {
                try {
                    Show removido = pilha.remover();
                    System.out.println("(R) " + removido.getShow_id() + " ## " + removido.getTitle());
                } catch (Exception e) {
                    System.out.println("Erro ao remover: " + e.getMessage());
                }
            } else if (operacao.startsWith("I ")) {
                String id = operacao.substring(2);
                for (Show s : catalogo) {
                    if (s.getShow_id().equals(id)) {
                        pilha.inserir(s.clone());
                        System.out.println("(I) " + s.getShow_id() + " ## " + s.getTitle());
                        break;
                    }
                }
            }
        }

        /* Impressão dos elementos ordenados
       
        for (Show show : catalogo) {
            show.imprimir();
        }
        */ 
        scanner.close();
    }
}
