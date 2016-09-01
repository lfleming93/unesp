json.array!(@treatments) do |treatment|
  json.extract! treatment, :id, :installation_id, :employee_id
  json.url treatment_url(treatment, format: :json)
end
