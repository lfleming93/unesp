json.array!(@evaluations) do |evaluation|
  json.extract! evaluation, :id, :data, :observacoes, :altura, :comprimento, :peso, :envergadura, :animal_id, :funcionario_id
  json.url evaluation_url(evaluation, format: :json)
end
