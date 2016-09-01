class CreateSponsors < ActiveRecord::Migration
  def change
    create_table :sponsors do |t|
      t.string :bairro
      t.string :rua
      t.string :cidade
      t.string :nome_da_empresa
      t.string :produto
      t.decimal :montante
      t.string :telefone

      t.timestamps null: false
    end
  end
end
