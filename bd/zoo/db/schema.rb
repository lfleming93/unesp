# encoding: UTF-8
# This file is auto-generated from the current state of the database. Instead
# of editing this file, please use the migrations feature of Active Record to
# incrementally modify your database, and then regenerate this schema definition.
#
# Note that this schema.rb definition is the authoritative source for your
# database schema. If you need to create the application database on another
# system, you should be using db:schema:load, not running all the migrations
# from scratch. The latter is a flawed and unsustainable approach (the more migrations
# you'll amass, the slower it'll run and the greater likelihood for issues).
#
# It's strongly recommended that you check this file into your version control system.

ActiveRecord::Schema.define(version: 20160117214026) do

  create_table "animals", force: :cascade do |t|
    t.date     "data_entrada"
    t.date     "data_nasc"
    t.string   "especie"
    t.string   "filiacao"
    t.string   "nome"
    t.string   "categoria"
    t.integer  "installation_id"
    t.datetime "created_at",      null: false
    t.datetime "updated_at",      null: false
  end

  add_index "animals", ["installation_id"], name: "index_animals_on_installation_id"

  create_table "employees", force: :cascade do |t|
    t.string   "funcao"
    t.date     "data_nasc"
    t.date     "data_cont"
    t.string   "sexo"
    t.string   "bairro"
    t.string   "cidade"
    t.string   "rua"
    t.integer  "n_carteira_trabalho"
    t.string   "nome"
    t.string   "telefone"
    t.string   "especialidade"
    t.datetime "created_at",          null: false
    t.datetime "updated_at",          null: false
  end

  create_table "evaluations", force: :cascade do |t|
    t.date     "data"
    t.string   "observacoes"
    t.decimal  "altura"
    t.decimal  "comprimento"
    t.decimal  "peso"
    t.decimal  "envergadura"
    t.integer  "animal_id"
    t.integer  "employee_id"
    t.datetime "created_at",  null: false
    t.datetime "updated_at",  null: false
  end

  add_index "evaluations", ["animal_id"], name: "index_evaluations_on_animal_id"
  add_index "evaluations", ["employee_id"], name: "index_evaluations_on_employee_id"

  create_table "installations", force: :cascade do |t|
    t.string   "descricao"
    t.decimal  "area"
    t.decimal  "volume"
    t.string   "localizacao"
    t.string   "tipo"
    t.integer  "sponsor_id"
    t.datetime "created_at",  null: false
    t.datetime "updated_at",  null: false
  end

  add_index "installations", ["sponsor_id"], name: "index_installations_on_sponsor_id"

  create_table "sponsors", force: :cascade do |t|
    t.string   "bairro"
    t.string   "rua"
    t.string   "cidade"
    t.string   "nome_da_empresa"
    t.string   "produto"
    t.decimal  "montante"
    t.string   "telefone"
    t.datetime "created_at",      null: false
    t.datetime "updated_at",      null: false
  end

  create_table "treatments", force: :cascade do |t|
    t.integer  "installation_id"
    t.integer  "employee_id"
    t.datetime "created_at",      null: false
    t.datetime "updated_at",      null: false
  end

  add_index "treatments", ["employee_id"], name: "index_treatments_on_employee_id"
  add_index "treatments", ["installation_id"], name: "index_treatments_on_installation_id"

end
