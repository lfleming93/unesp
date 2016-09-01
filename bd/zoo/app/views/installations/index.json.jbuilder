json.array!(@installations) do |installation|
  json.extract! installation, :id, :descricao, :area, :volume, :localizacao, :tipo, :sponsor_id
  json.url installation_url(installation, format: :json)
end
