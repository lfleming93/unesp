require 'test_helper'

class SponsorsControllerTest < ActionController::TestCase
  setup do
    @sponsor = sponsors(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:sponsors)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create sponsor" do
    assert_difference('Sponsor.count') do
      post :create, sponsor: { bairro: @sponsor.bairro, cidade: @sponsor.cidade, montante: @sponsor.montante, nome_da_empresa: @sponsor.nome_da_empresa, produto: @sponsor.produto, rua: @sponsor.rua, telefone: @sponsor.telefone }
    end

    assert_redirected_to sponsor_path(assigns(:sponsor))
  end

  test "should show sponsor" do
    get :show, id: @sponsor
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @sponsor
    assert_response :success
  end

  test "should update sponsor" do
    patch :update, id: @sponsor, sponsor: { bairro: @sponsor.bairro, cidade: @sponsor.cidade, montante: @sponsor.montante, nome_da_empresa: @sponsor.nome_da_empresa, produto: @sponsor.produto, rua: @sponsor.rua, telefone: @sponsor.telefone }
    assert_redirected_to sponsor_path(assigns(:sponsor))
  end

  test "should destroy sponsor" do
    assert_difference('Sponsor.count', -1) do
      delete :destroy, id: @sponsor
    end

    assert_redirected_to sponsors_path
  end
end
