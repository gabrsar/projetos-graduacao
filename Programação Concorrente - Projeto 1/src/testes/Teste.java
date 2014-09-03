package testes;

import java.util.Date;

public class Teste {
	int tamanhoMatriz;
	int numeroThreads;
	int numeroDeSequencia;

	Date inicio;
	Date conclusao;

	public Teste(int tamanho, int threads, int sequencia, Date i, Date c) {
		tamanhoMatriz = tamanho;
		numeroThreads = threads;
		numeroDeSequencia = sequencia;
		inicio = i;
		conclusao = c;
	}

	public int getTamanhoMatriz() {
		return tamanhoMatriz;
	}

	public void setTamanhoMatriz(int tamanhoMatriz) {
		this.tamanhoMatriz = tamanhoMatriz;
	}

	public int getNumeroThreads() {
		return numeroThreads;
	}

	public void setNumeroThreads(int numeroThreads) {
		this.numeroThreads = numeroThreads;
	}

	public int getNumeroDeSequencia() {
		return numeroDeSequencia;
	}

	public void setNumeroDeSequencia(int numeroDeSequencia) {
		this.numeroDeSequencia = numeroDeSequencia;
	}

	public Date getInicio() {
		return inicio;
	}

	public void setInicio(Date inicio) {
		this.inicio = inicio;
	}

	public Date getConclusao() {
		return conclusao;
	}

	public void setConclusao(Date conclusao) {
		this.conclusao = conclusao;
	}
	
	public double getDuracaoMS()
	{
		return conclusao.getTime() - inicio.getTime();
	}

        public String toString() {
            String teste = tamanhoMatriz + ", " + numeroThreads + ", " + getDuracaoMS();
            return teste;
    }
        
        

}
