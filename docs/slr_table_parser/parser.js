var gramaticaParsed = [];

$(function() {
	$('#form').on('submit', function(e) {
		e.preventDefault();

		parse(JSON.parse($('#in_jason').val()));

		return false;
	});
})

function parse(slrJson) {
	parseGramatica();

	var tableParse = $('#parse');
	var tableHeader = $('#parse thead tr').empty();
	var tableBody = $('#parse tbody').empty();

	var slrHead = ['TK_VOID','TK_INT','TK_FLOAT','TK_DOUBLE','TK_ID','TK_ABRE_PAR','TK_FECHAR_PAR','TK_PONTO_VIRG','TK_VIRGULA','TK_ATRIB','TK_ABRE_CHAVE','TK_FECHA_CHAVE','TK_WHILE','TK_DO','TK_FOR','TK_RETURN','TK_CONTINUE','TK_BREAK','TK_IF','TK_ELSE','TK_SWITCH','TK_CASE','TK_CONST','TK_DOIS_PONTOS','TK_DEFAULT','TK_INTERROGACAO','TK_OU','TK_E','TK_BITWISE_OU','TK_BITWISE_XOU','TK_BITWISE_E','TK_IGUAL','TK_DIFERENTE','TK_MAIOR','TK_MENOR','TK_MAIOR_IGUAL','TK_MENOR_IGUAL','TK_BITWISE_ESQ','TK_BITWISE_DIR','TK_SOMA','TK_SUB','TK_MULT','TK_DIV','TK_RESTO','TK_INCREMENTO','TK_DECREMENTO','TK_INCR_MULT','TK_INCR_DIV','TK_INCR_RESTO','TK_INCR_SOMA','TK_INCR_SUB','TK_BITWISE_COMPL','TK_NEGACAO','$','E\'','E','INICIO','TIPO','DEFINICAO_FUNCAO','DEFINICAO_FUNCAO_CORPO','DEFINICAO_PARAMETROS','DEFINICAO_PARAMETRO','DEFINICAO_VARIAVEIS','DEFINICAO_VARIAVEL_LISTA','DEFINICAO_VARIAVEL','COMANDO','COMANDO_BLOCO','COMANDO_LISTA','COMANDO_EXPRESSAO','COMANDO_ITERACAO','COMANDO_ITERACAO_COMANDO_FOR','COMANDO_PULO','COMANDO_CONDICAO','COMANDO_CONDICAO_SWITCH_BLOCO','COMANDO_CONDICAO_LISTA_CASE','COMANDO_CONDICAO_CASE','COMANDO_FUNCAO','COMANDO_FUNCAO_LISTA_PARAM','EXPRESSAO','EXPRESSAO_CONDICIONAL','EXPRESSAO_LOGICA_OU','EXPRESSAO_LOGICA_E','EXPRESSAO_BITABIT_OU','EXPRESSAO_BITABIT_XOU','EXPRESSAO_BITABIT_E','EXPRESSAO_IGUALDADE','EXPRESSAO_RELACIONAL','EXPRESSAO_BITABIT_TROCA','EXPRESSAO_SOMA','EXPRESSAO_MULT','EXPRESSAO_ATRIB','EXPRESSAO_UNARIA','EXPRESSAO_PRIMARIA','OPERADOR_ATRIB','OPERADOR_UNARIO'];

	$('#qtd_elem_gramatica').text(slrHead.length);
	$('#qtd_elem_producao').text(slrJson.length);

	tableHeader.append($('<td>'));
	$.each(slrHead, function(i, val) {
		tableHeader.append($('<td>', {text: val, 'data-col': i, 'data-val': val}));
	});

	$.each(slrJson, function(iLinha, tkEstados) {
		var tr = $('<tr>');
		tr.append($('<td>', {text: iLinha}));

		$.each(slrHead, function(i) {
			var t = '';

			if (i == 0) {
				t = '{';
			}

			t += '<span data-campo="val">NULL</span>' + (i == slrHead.length - 1 ? '' : ',');

			if (i == slrHead.length - 1) {
				t += '}, \\';
			}

			tr.append($('<td>', {html: t, 'data-col': i}));
		});

		$.each(tkEstados, function(tk, estados) {
			if (tk == 'index') {
				return;
			}

			var col = tableHeader.find('[data-val="' + tk + '"]').attr('data-col');
			tr.find('td[data-col="' + col + '"] span[data-campo="val"]').empty().append(parseEstado(tk, estados));
		});

		tableBody.append(tr);
	});
}

function parseEstado(tk, estados) {
	var estadoParsed = '';
	var estado = estados[0];

	if (estado.actionType == 'r' && estado.actionValue == 0) {
		estadoParsed = 'cel(\'A\', -1, 1)';
	} else {
		estadoParsed = 'cel(\'' + (estado.actionType == '' ? ' ' : estado.actionType.toUpperCase()) + '\', ' + estado.actionValue + ', ' + qtdDesempilhar(estado.actionType.toUpperCase(), estado.actionValue) + ')';
	}

	return estadoParsed;
}

function parseGramatica() {
	var gramatica = $('#gramatica').val();
	var gramaticaLinhas = gramatica.split('\n');
	var gramaticaCount = {};
	var seq = 0;

	$.each(gramaticaLinhas, function(i, linha) {
		var linhaSplit = linha.split('->');
		var descProducao = linhaSplit[0].trim();
		var producao = linhaSplit[1].trim();

		gramaticaParsed.push(producao.split(' ').length);

		if (gramaticaCount[descProducao]) {
			gramaticaCount[descProducao].push(seq);
		} else {
			gramaticaCount[descProducao] = [seq];
		}

		seq--;
	});

	$.each(gramaticaCount, function(descProducao, counts) {
		$('#gramatica_define').append('#define ' + descProducao + ' {' + counts.join(',') + '}<br />');
	});
}

function qtdDesempilhar(tipo, iProd) {
	return (tipo == 'S' || tipo == '' ? 1 : gramaticaParsed[iProd]);
}