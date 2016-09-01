json.array!(@employees) do |employee|
  json.extract! employee, :id, :funcao, :data_nasc, :data_cont, :sexo, :bairro, :cidade, :rua, :n_carteira_trabalho, :nome, :telefone, :especialidade
  json.url employee_url(employee, format: :json)
end
