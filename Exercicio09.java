import java.io.*;
import java.text.SimpleDateFormat;
import java.util.*;

class Show {
    //atributos
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

    public Show() { //contrutor padrão
        this.show_id = this.type = this.title = this.director = this.country = this.rating = this.duration = "NaN";
        this.cast = new String[0];
        this.listed_in = new String[0];
        this.date_added = null;
        this.release_year = -1;
    }

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

    public void lerLinha(String linhaCSV) {
        try {
            List<String> camposSeparados = new ArrayList<>();
            StringBuilder campoAtual = new StringBuilder();
            boolean entreAspas = false;

            for (int i = 0; i < linhaCSV.length(); i++) {
                char ch = linhaCSV.charAt(i);

                if (ch == '"') {
                    entreAspas = !entreAspas;
                } else if (ch == ',' && !entreAspas) {
                    camposSeparados.add(campoAtual.toString().trim());
                    campoAtual.setLength(0);
                } else {
                    campoAtual.append(ch);
                }
            }

            camposSeparados.add(campoAtual.toString().trim());

            String[] campos = camposSeparados.toArray(new String[0]);

            this.show_id = campos[0];
            this.type = campos[1];
            this.title = campos[2].replace("\"", "");
            this.director = campos[3].isEmpty() ? "NaN" : campos[3];
            this.cast = campos[4].isEmpty() ? new String[]{"NaN"} : campos[4].split(",\\s*");
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
            this.listed_in = campos[10].isEmpty() ? new String[]{"NaN"} : campos[10].split(",\\s*");
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
            SimpleDateFormat formatoSaida = new SimpleDateFormat("MMMM d, yyyy", Locale.US);
            System.out.print(formatoSaida.format(date_added));
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


/**
 * Celula (pilha, lista e fila dinamica)
 * @author Max do Val Machado
 * @version 2 01/2015
 */
class Celula {
	public Show elemento; // Elemento inserido na celula.
	public Celula prox; // Aponta a celula prox.

	/**
	 * Construtor da classe.
	 */
	public Celula() {
		this(null);
	}

	/**
	 * Construtor da classe.
	 * @param elemento int inserido na celula.
	 */
	public Celula(Show elemento) {
        this.elemento = elemento;
        this.prox = null;
	}
}

/**
 * Pilha dinamica
 * 
 * @author Max do Val Machado
 * @version 2 01/2015
 */
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

	public void mostrar() {
        int controle = 0;
        for(Celula i = topo.prox; i != null; i = i.prox){
            controle++;
        }
		for (Celula i = topo; i != null; i = i.prox) {
            System.out.print("["+controle+"] ");
            i.elemento.imprimir();
            controle--;
		}
	}

	public Show getMax() {
		Show max = topo.elemento;
		for (Celula i = topo.prox; i != null; i = i.prox) {
			if (i.elemento.getRelease_year() > max.getRelease_year())
				max = i.elemento;
		}
		return max;
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
}


public class Exercicio09 {
    static List<Show> acervo = new ArrayList<>();

    public static void carregarDadosCSV() {
        String arquivo = "/tmp/disneyplus.csv";
        try {
            BufferedReader leitor = new BufferedReader(new FileReader(arquivo));
            leitor.readLine(); // ignora o cabeçalho

            String linha;
            while ((linha = leitor.readLine()) != null) {
                Show show = new Show();
                show.lerLinha(linha);
                acervo.add(show);
            }

            leitor.close();
        } catch (IOException e) {
            System.out.println("Erro ao ler arquivo: " + e.getMessage());
        }
    }

    public static boolean comandoFim(String entrada) {
        return entrada.equals("FIM");
    }

    public static void main(String[] args) {
        carregarDadosCSV();
        Scanner teclado = new Scanner(System.in);
        String entrada = teclado.nextLine();
        Pilha pilha = new Pilha();

        while (!comandoFim(entrada)) {
            for (int i = 0; i < acervo.size(); i++) {
                Show s = acervo.get(i);
                if (s.getShow_id().equals(entrada)) {
                    try {
                        pilha.inserir(s);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    break;
                }
            }
            entrada = teclado.nextLine();
        }

        int numOperacoes = teclado.nextInt();

        for (int i = 0; i < numOperacoes; i++) {
            entrada = teclado.next();
            String codigo;
            if (entrada.equals("I")) {
                codigo = teclado.next();
                Show encontrado = null;
                for (int j = 0; j < acervo.size(); j++) {
                    if (acervo.get(j).getShow_id().equals(codigo)) {
                        encontrado = acervo.get(j);
                        break;
                    }
                }
                try {
                    pilha.inserir(encontrado);
                } catch (Exception e) {}
            } else if (entrada.equals("R")) {
                Show removido = null;
                try {
                    removido = pilha.remover();
                } catch (Exception e) {}
                System.out.println("(R) " + removido.getTitle());
            }
        }

        pilha.mostrar();
        teclado.close();
    }
}
