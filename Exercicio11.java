import java.util.Scanner;

// Classe Celula para matriz dinâmica
class Celula {
    public int elemento;
    public Celula sup, inf, esq, dir;

    public Celula() {
        this(0);
    }

    public Celula(int elemento) {
        this.elemento = elemento;
        this.sup = this.inf = this.esq = this.dir = null;
    }
}

// Classe Matriz
class Matriz {
    private Celula inicio;
    private int linha, coluna;
 
    public Matriz() {
        this(3, 3);
    }

    public Matriz(int linha, int coluna) {
        this.linha = linha;
        this.coluna = coluna;
        if (linha <= 0 || coluna <= 0)
            return;

        // Cria a primeira célula da matriz
        inicio = new Celula();
        
        // Cria a primeira linha
        Celula lin = inicio;
        for (int j = 1; j < coluna; j++) {
            lin.dir = new Celula();
            lin.dir.esq = lin;
            lin = lin.dir;
        }

        // Outras linhas
        Celula sup = inicio;
        for (int i = 1; i < linha; i++) {
            Celula novaLinha = new Celula();
            sup.inf = novaLinha;
            novaLinha.sup = sup;
            Celula atual = novaLinha;
            Celula supDir = sup.dir;
            for (int j = 1; j < coluna; j++) {
                atual.dir = new Celula();
                atual.dir.esq = atual;
                atual = atual.dir;
                if (supDir != null) {
                    supDir.inf = atual;
                    atual.sup = supDir;
                    supDir = supDir.dir;
                }
            }
            sup = sup.inf;
        }
    }

    // Lê a matriz 
    public void ler(Scanner sc) {
        Celula lin = inicio;
        for (int i = 0; i < linha; i++, lin = lin.inf) {
            Celula col = lin;
            for (int j = 0; j < coluna; j++, col = col.dir) {
                col.elemento = sc.nextInt();
            }
        }
    }

    // Soma de matrizes
    public Matriz soma(Matriz m) {
        Matriz resp = null;

        if (this.linha == m.linha && this.coluna == m.coluna) {
            resp = new Matriz(this.linha, this.coluna);
            Celula aLinha = this.inicio, bLinha = m.inicio, cLinha = resp.inicio;
            for (int i = 0; i < this.linha; i++, aLinha = aLinha.inf, bLinha = bLinha.inf, cLinha = cLinha.inf) {
                Celula a = aLinha, b = bLinha, c = cLinha;
                for (int j = 0; j < this.coluna; j++, a = a.dir, b = b.dir, c = c.dir) {
                    c.elemento = a.elemento + b.elemento;
                }
            }
        }

        return resp;
    }

    // Multiplicação de matrizes
    public Matriz multiplicacao(Matriz m) {
        Matriz resp = null;

        if (this.coluna == m.linha) {
            resp = new Matriz(this.linha, m.coluna);
            for (int i = 0; i < this.linha; i++) {
                for (int j = 0; j < m.coluna; j++) {
                    int soma = 0;
                    for (int k = 0; k < this.coluna; k++) {
                        soma += this.get(i, k) * m.get(k, j);
                    }
                    resp.set(i, j, soma);
                }
            }
        }

        return resp;
    }

    // Retorna true se for quadrada
    public boolean isQuadrada() {
        return (this.linha == this.coluna);
    }

    // Mostra diagonal principal
    public void mostrarDiagonalPrincipal() {
        if (isQuadrada()) {
            Celula novCelula = inicio;
            for (int i = 0; i < linha; i++) {
                System.out.print(novCelula.elemento);
                if (i < linha - 1)
                    System.out.print(" ");
                if (novCelula.inf != null && novCelula.dir != null)
                    novCelula = novCelula.inf.dir;
            }
            System.out.println();
        }
    }

    // Mostra diagonal secundária
    public void mostrarDiagonalSecundaria() {
        if (isQuadrada()) {
            // Vai para o canto superior direito
            Celula novCelula = inicio;
            for (int j = 1; j < coluna; j++) novCelula = novCelula.dir;
            for (int i = 0; i < linha; i++) {
                System.out.print(novCelula.elemento);
                if (i < linha - 1)
                    System.out.print(" ");
                if (novCelula.inf != null && novCelula.esq != null)
                    novCelula = novCelula.inf.esq;
            }
            System.out.println();
        }
    }

    // Retorna o valor na posição (i,j)
    public int get(int i, int j) {
        Celula novCelula = inicio;
        for (int linha = 0; linha < i; linha++) novCelula = novCelula.inf;
        for (int col = 0; col < j; col++) novCelula = novCelula.dir;
        return novCelula.elemento;
    }

    // Define o valor na posição (i,j)
    public void set(int i, int j, int v) {
        Celula novCelula = inicio;
        for (int linha = 0; linha < i; linha++) novCelula = novCelula.inf;
        for (int col = 0; col < j; col++) novCelula = novCelula.dir;
        novCelula.elemento = v;
    }

    // Imprime a matriz
    public void mostrar() {
        Celula lin = inicio;
        for (int i = 0; i < linha; i++, lin = lin.inf) {
            Celula col = lin;
            for (int j = 0; j < coluna; j++, col = col.dir) {
                System.out.print(col.elemento);
                if (j < coluna - 1)
                    System.out.print(" ");
            }
            System.out.println();
        }
    }
}

// Main
public class Exercicio11 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int opcoes = scanner.nextInt();

        for (int t = 0; t < opcoes; t++) {
            // Primeira matriz
            int linha1 = scanner.nextInt();
            int coluna1 = scanner.nextInt();
            Matriz m1 = new Matriz(linha1, coluna1);
            m1.ler(scanner);

            // Segunda matriz
            int linha2 = scanner.nextInt();
            int coluna2 = scanner.nextInt();
            Matriz m2 = new Matriz(linha2, coluna2);
            m2.ler(scanner);

            // Diagonais da primeira matriz
            m1.mostrarDiagonalPrincipal();
            m1.mostrarDiagonalSecundaria();

            // Soma das matrizes
            Matriz soma = m1.soma(m2);
            if (soma != null)
                soma.mostrar();

            // Multiplicação das matrizes
            Matriz mult = m1.multiplicacao(m2);
            if (mult != null)
                mult.mostrar();
        }
        scanner.close();
    }
}