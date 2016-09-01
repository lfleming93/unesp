json.array!(@sponsors) do |sponsor|
  json.extract! sponsor, :id, :bairro, :rua, :cidade, :nome_da_empresa, :produto, :montante, :telefone
  json.url sponsor_url(sponsor, format: :json)
end
