class CreateAnimals < ActiveRecord::Migration
  def change
    create_table :animals do |t|
      t.date :data_entrada
      t.date :data_nasc
      t.string :especie
      t.string :filiacao
      t.string :nome
      t.string :categoria
      t.references :installation, index: true, foreign_key: true

      t.timestamps null: false
    end
  end
end
