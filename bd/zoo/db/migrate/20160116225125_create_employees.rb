class CreateEmployees < ActiveRecord::Migration
  def change
    create_table :employees do |t|
      t.string :funcao
      t.date :data_nasc
      t.date :data_cont
      t.string :sexo
      t.string :bairro
      t.string :cidade
      t.string :rua
      t.integer :n_carteira_trabalho
      t.string :nome
      t.string :telefone
      t.string :especialidade

      t.timestamps null: false
    end
  end
end
