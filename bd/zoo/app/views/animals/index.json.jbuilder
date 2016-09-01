json.array!(@animals) do |animal|
  json.extract! animal, :id, :data_entrada, :data_nasc, :especie, :filiacao, :nome, :categoria, :installation_id
  json.url animal_url(animal, format: :json)
end
