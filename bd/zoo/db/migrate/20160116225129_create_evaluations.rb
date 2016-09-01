class CreateEvaluations < ActiveRecord::Migration
  def change
    create_table :evaluations do |t|
      t.date :data
      t.string :observacoes
      t.decimal :altura
      t.decimal :comprimento
      t.decimal :peso
      t.decimal :envergadura
      t.references :animal, index: true, foreign_key: true
      t.references :funcionario, index: true, foreign_key: true

      t.timestamps null: false
    end
  end
end
